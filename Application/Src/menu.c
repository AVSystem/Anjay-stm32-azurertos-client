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

#include <stdio.h>
#include <string.h>

#include <avsystem/commons/avs_log.h>
#include <avsystem/commons/avs_utils.h>

#include "config_persistence.h"
#include "console.h"
#include "default_config.h"

#define LOG(level, ...) avs_log(menu, level, __VA_ARGS__)

config_t g_config;

typedef enum {
    OPTION_SAVE_EXIT,
    OPTION_SERVER_URI,
    OPTION_BOOTSTRAP,
    OPTION_SECURITY,
    OPTION_ENDPOINT_NAME,
    OPTION_PUBLIC_CERT_OR_PSK_IDENTITY,
    OPTION_PRIVATE_CERT_OR_PSK_KEY,
    OPTION_APN,
    OPTION_APN_USERNAME,
    OPTION_APN_PASSWORWD,
    OPTION_DISCARD_CHANGES,
    OPTION_FACTORY_RESET,
    _OPTION_END
} menu_option_id_t;

typedef struct {
    const char *const description;
    char *const value;
    size_t value_capacity;
} menu_option_t;

static menu_option_t OPTIONS[] = {
    [OPTION_SAVE_EXIT] = { "Save & Exit", NULL, 0 },
    [OPTION_SERVER_URI] = { "LwM2M Server URI", g_config.server_uri,
                            sizeof(g_config.server_uri) },
    [OPTION_BOOTSTRAP] = { "Bootstrap (y/n)", g_config.bootstrap,
                           sizeof(g_config.bootstrap) },
    [OPTION_SECURITY] = { "Security (none/psk/cert)", g_config.security,
                          sizeof(g_config.security) },
    [OPTION_ENDPOINT_NAME] = { "Endpoint name", g_config.endpoint_name,
                               sizeof(g_config.endpoint_name) },
    [OPTION_PUBLIC_CERT_OR_PSK_IDENTITY] =
            { "Public cert or PSK identity",
              g_config.public_cert_or_psk_identity,
              sizeof(g_config.public_cert_or_psk_identity) },
    [OPTION_PRIVATE_CERT_OR_PSK_KEY] =
            { "Private cert or PSK", g_config.private_cert_or_psk_key,
              sizeof(g_config.private_cert_or_psk_key) },
    [OPTION_APN] = { "APN", g_config.apn, sizeof(g_config.apn) },
    [OPTION_APN_USERNAME] = { "APN username", g_config.apn_username,
                              sizeof(g_config.apn_username) },
    [OPTION_APN_PASSWORWD] = { "APN password", g_config.apn_password,
                               sizeof(g_config.apn_password) },
    [OPTION_DISCARD_CHANGES] = { "Discard changes", NULL, 0 },
    [OPTION_FACTORY_RESET] = { "Factory reset", NULL, 0 },
};

static void print_menu(void) {
    console_printf("\r\n");
    console_printf("####################################################\r\n");
    console_printf("################ Configuration menu ################\r\n");
    console_printf("####################################################\r\n");
    for (int i = 0; i < _OPTION_END; i++) {
        console_printf("  %d. %s", i + 1, OPTIONS[i].description);
        if (OPTIONS[i].value) {
            console_write("\r\n     ", 7);
            console_write(OPTIONS[i].value, strlen(OPTIONS[i].value));
        }
        console_printf("\r\n");
    }
    console_printf("Select option (1 - %d): ", _OPTION_END);
}

static int get_option_id(menu_option_id_t *option_id) {
    char buffer[3];
    int id;
    console_read_line(buffer, sizeof(buffer), '\r');
    if (sscanf(buffer, "%d", &id) != 1 || id < 1 || id > _OPTION_END) {
        return -1;
    }
    const int menu_option_id_offset = 1;
    *option_id = id - menu_option_id_offset;
    return 0;
}

#define EOT 4 // End of transmission (CTRL+D)
static void get_value(menu_option_id_t option_id) {
    console_printf("Enter value for `%s`, use CTRL+D to accept:\r\n",
                   OPTIONS[option_id].description);
    console_read_line(OPTIONS[option_id].value,
                      OPTIONS[option_id].value_capacity, EOT);
}

static bool get_confirmation(void) {
    console_printf("Are you sure? ('y' to confirm) ");
    char answer[2];
    console_read_line(answer, sizeof(answer), '\r');
    return answer[0] == 'y';
}

static void config_restore_defaults(void) {
    g_config = (config_t) {
        .server_uri = DEFAULT_SERVER_URI,
        .private_cert_or_psk_key = DEFAULT_PSK,
        .bootstrap = DEFAULT_BOOTSTRAP,
        .security = DEFAULT_SECURITY,
        .apn = DEFAULT_APN,
        .apn_username = DEFAULT_APN_USERNAME,
        .apn_password = DEFAULT_APN_PASSWORD
    };
    generate_default_endpoint_name(g_config.endpoint_name,
                                   sizeof(g_config.endpoint_name));
    strcpy(g_config.public_cert_or_psk_identity, g_config.endpoint_name);
}

static void enter_menu(void) {
    bool changed = false;
    bool menu_running = true;
    while (menu_running) {
        print_menu();
        menu_option_id_t option_id;
        if (get_option_id(&option_id)) {
            console_printf("Invalid choice\r\n");
            continue;
        }
        if (OPTIONS[option_id].value) {
            get_value(option_id);
            changed = true;
        } else {
            switch (option_id) {
            case OPTION_DISCARD_CHANGES:
                if (get_confirmation()) {
                    console_printf("Discarding changes...\r\n");
                    if (config_load(&g_config)) {
                        LOG(WARNING, "Could not restore current configuration");
                    } else {
                        changed = false;
                    }
                }
                break;
            case OPTION_FACTORY_RESET:
                if (get_confirmation()) {
                    changed = true;
                    config_restore_defaults();
                    console_printf("Performing factory reset...\r\n");
                }
                break;
            case OPTION_SAVE_EXIT:
                menu_running = false;
                if (changed) {
                    console_printf("\r\nSaving config...\r\n");
                    if (config_save(&g_config)) {
                        LOG(WARNING, "Could not save config");
                    }
                }
                console_printf("\r\nExiting menu...\r\n");
                break;
            default:
                AVS_UNREACHABLE("invalid option id");
            }
        }
    }
}

void menu_init(void) {
    console_init();
    int res = 0;
    console_printf("\r\nLoading config... ");
    if ((res = config_load(&g_config))) {
        console_printf("Failed: %d", res);
        config_restore_defaults();
    }
    console_printf(
            "\r\nPress any key in 3 seconds to enter config menu...\r\n");
    if (console_wait_for_key_press(3000)) {
        enter_menu();
    }
}

const char *config_get_server_uri(void) {
    return g_config.server_uri;
}

const char *config_get_endpoint_name(void) {
    return g_config.endpoint_name;
}

const char *config_get_apn(void) {
    return g_config.apn;
}

const char *config_get_apn_username(void) {
    return g_config.apn_username;
}

const char *config_get_apn_password(void) {
    return g_config.apn_password;
}
