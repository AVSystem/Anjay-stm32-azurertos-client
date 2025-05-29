/*
 * Copyright 2023-2025 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>

#include <cellular_control_api.h>
#include <cellular_service_datacache.h>
#include <cmsis_os.h>
#include <rng.h>

#include <avsystem/commons/avs_log.h>

#include <anjay/anjay.h>
#include <anjay/security.h>
#include <anjay/server.h>

#include <application.h>
#include <device_object.h>
#include <firmware_update.h>
#include <sensor_objects.h>

#include "config_persistence.h"
#include "menu.h"
#include "trace_interface.h"

#define LOG(level, ...) avs_log(app, level, __VA_ARGS__)

static anjay_t *anjay;

static void dc_cellular_callback(dc_com_event_id_t dc_event_id,
                                 const void *user_arg) {
    (void) user_arg;

    if (dc_event_id == DC_CELLULAR_NIFMAN_INFO) {
        dc_nifman_info_t dc_nifman_info;
        (void) dc_com_read(&dc_com_db, DC_CELLULAR_NIFMAN_INFO,
                           (void *) &dc_nifman_info, sizeof(dc_nifman_info));
        if (dc_nifman_info.rt_state == DC_SERVICE_ON) {
            LOG(INFO, "network is up");
            anjay_transport_exit_offline(anjay, ANJAY_TRANSPORT_SET_ALL);
        } else {
            LOG(INFO, "network is down");
            anjay_transport_enter_offline(anjay, ANJAY_TRANSPORT_SET_ALL);
        }
    } else if (dc_event_id == DC_CELLULAR_CONFIG) {
        dc_cellular_params_t dc_cellular_params;
        (void) dc_com_read(&dc_com_db, DC_CELLULAR_CONFIG,
                           (void *) &dc_cellular_params,
                           sizeof(dc_cellular_params));
        if (dc_cellular_params.rt_state == DC_SERVICE_ON) {
            LOG(INFO, "cellular configuration is updated");
        }
    }
}

static int configure_network_availability_fence(void) {
    if (dc_com_register_gen_event_cb(&dc_com_db, dc_cellular_callback, NULL)
            == DC_COM_INVALID_ENTRY) {
        return -1;
    }

    return 0;
}

static int configure_modem(void) {
    dc_cellular_params_t cellular_params;
    memset(&cellular_params, 0, sizeof(cellular_params));

    if (dc_com_read(&dc_com_db, DC_CELLULAR_CONFIG, &cellular_params,
                    sizeof(cellular_params))
            != DC_COM_OK) {
        return -1;
    }

    const uint8_t sim_slot_index = 0;
    const dc_sim_slot_t *sim_slot = &cellular_params.sim_slot[sim_slot_index];

    strcpy((char *) sim_slot->apn, config_get_apn());
    strcpy((char *) sim_slot->username, config_get_apn_username());
    strcpy((char *) sim_slot->password, config_get_apn_password());

    if (dc_com_write(&dc_com_db, DC_CELLULAR_CONFIG, &cellular_params,
                     sizeof(cellular_params))
            != DC_COM_OK) {
        return -1;
    }

    return 0;
}

static int
entropy_callback(unsigned char *out_buf, size_t out_buf_len, void *user_ptr) {
    uint32_t random_number;
    for (size_t i = 0; i < out_buf_len / sizeof(random_number); i++) {
        if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) != HAL_OK) {
            return -1;
        }
        memcpy(out_buf, &random_number, sizeof(random_number));
        out_buf += sizeof(random_number);
    }

    size_t last_chunk_size = out_buf_len % sizeof(random_number);
    if (last_chunk_size) {
        if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) != HAL_OK) {
            return -1;
        }
        memcpy(out_buf, &random_number, last_chunk_size);
    }

    return 0;
}

static void update_objects(avs_sched_t *sched, const void *anjay_ptr) {
    anjay_t *anjay = *(anjay_t *const *) anjay_ptr;

    device_object_update(anjay);
    basic_sensor_objects_update(anjay);
    three_axis_sensor_objects_update(anjay);

    AVS_SCHED_DELAYED(sched, NULL, avs_time_duration_from_scalar(1, AVS_TIME_S),
                      update_objects, &anjay, sizeof(anjay));
}

static void
log_handler(avs_log_level_t level, const char *module, const char *message) {
    traceIF_uartPrintForce(DBG_CHAN_APPLICATION, (uint8_t *) message,
                           strlen(message));
    traceIF_uartPrintForce(DBG_CHAN_APPLICATION, (uint8_t *) "\r\n", 2);
}

static int application_run(void) {
    menu_init();
    avs_log_set_handler(log_handler);

    cellular_init();
    if (configure_modem()) {
        LOG(ERROR, "Modem configuration error");
        return -1;
    }
    if (configure_network_availability_fence()) {
        LOG(ERROR, "Event callback registration failed");
        return -1;
    }

    avs_crypto_prng_ctx_t *prng_ctx =
            avs_crypto_prng_new(entropy_callback, NULL);
    assert(prng_ctx);

    const anjay_configuration_t config = {
        .endpoint_name = config_get_endpoint_name(),
        .in_buffer_size = 2048,
        .out_buffer_size = 2048,
        .msg_cache_size = 2048,
        .prng_ctx = prng_ctx
    };

    anjay = anjay_new(&config);
    if (!anjay) {
        LOG(ERROR, "creation of anjay failed");
        return -1;
    }

    if (anjay_transport_enter_offline(anjay, ANJAY_TRANSPORT_SET_ALL)) {
        LOG(ERROR, "anjay_transport_enter_offline failed");
        return -1;
    }

    cellular_start();

    if (anjay_security_object_install(anjay)) {
        LOG(ERROR, "security object install failed");
        return -1;
    }

    anjay_security_instance_t security_instance = {
        .ssid = 1,
        .server_uri = config_get_server_uri()
    };

    if (strcmp(g_config.security, "psk") == 0) {
        security_instance.security_mode = ANJAY_SECURITY_PSK;
    } else if (strcmp(g_config.security, "cert") == 0) {
        security_instance.security_mode = ANJAY_SECURITY_CERTIFICATE;
    } else {
        security_instance.security_mode = ANJAY_SECURITY_NOSEC;
    }

    if (security_instance.security_mode != ANJAY_SECURITY_NOSEC) {
        security_instance.public_cert_or_psk_identity =
                (uint8_t *) g_config.public_cert_or_psk_identity;
        security_instance.public_cert_or_psk_identity_size =
                strlen(g_config.public_cert_or_psk_identity);
        security_instance.private_cert_or_psk_key =
                (uint8_t *) g_config.private_cert_or_psk_key;
        security_instance.private_cert_or_psk_key_size =
                strlen(g_config.private_cert_or_psk_key);
        if (strcmp(g_config.security, "cert") == 0) {
            security_instance.public_cert_or_psk_identity_size += 1;
            security_instance.private_cert_or_psk_key_size += 1;
        }
    }

    if (g_config.bootstrap[0] == 'y') {
        security_instance.bootstrap_server = true;
    } else {
        security_instance.bootstrap_server = false;
    }

    anjay_iid_t security_instance_id = ANJAY_ID_INVALID;

    if (anjay_security_object_add_instance(anjay, &security_instance,
                                           &security_instance_id)) {
        LOG(ERROR, "failed to add instance of security object");
        return -1;
    }
    if (anjay_server_object_install(anjay)) {
        LOG(ERROR, "server object install failed");
        return -1;
    }

    const anjay_server_instance_t server_instance = {
        .ssid = 1,
        .lifetime = 50,
        .default_min_period = -1,
        .default_max_period = -1,
        .disable_timeout = -1,
        .binding = "U"
    };

    anjay_iid_t server_instance_id = ANJAY_ID_INVALID;

    if (g_config.bootstrap[0] != 'y') {
        if (anjay_server_object_add_instance(anjay, &server_instance,
                                             &server_instance_id)) {
            LOG(ERROR, "failed to add instance of server object");
            return -1;
        }
    }

    basic_sensor_objects_install(anjay);
    three_axis_sensor_objects_install(anjay);
#ifdef USE_FW_UPDATE
    fw_update_install(anjay);
#endif /* USE_FW_UPDATE */
    if (device_object_install(anjay)) {
        LOG(ERROR, "device object install failed");
        return -1;
    }

    update_objects(anjay_get_scheduler(anjay), &anjay);

    anjay_event_loop_run(anjay, avs_time_duration_from_scalar(1, AVS_TIME_S));

    return 0;
}

static void application_thread(void *arg) {
    (void) arg;

    application_run();

    LOG(ERROR, "Application failed!");
    LOG(ERROR, "Rebooting...");
    HAL_Delay(2000U);
    NVIC_SystemReset();
}

void application_init(void) {
    const osThreadAttr_t thread_attr = {
        .stack_size = 16 * 1024,
        .priority = osPriorityNormal
    };

    if (!osThreadNew(application_thread, NULL, &thread_attr)) {
        LOG(ERROR, "application_thread start failed");
    }
}
