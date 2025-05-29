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

#include <avsystem/commons/avs_log.h>

#include <string.h>

#include "b_u585i_iot02a_eeprom.h"
#include "config_persistence.h"

#define LOG(level, ...) avs_log(config, level, __VA_ARGS__)

static int eeprom_init() {
    static bool initialized = false;
    if (!initialized) {
        if (BSP_EEPROM_Init(0)) {
            return -1;
        }
        initialized = true;
    }
    return 0;
}

int config_save(const config_t *in_config) {
    if (eeprom_init()) {
        return -1;
    }
    if (BSP_EEPROM_WriteBuffer(0, (uint8_t *) MAGIC, MAGIC_BASE_ADDR,
                               MAGIC_SIZE)
            || BSP_EEPROM_IsDeviceReady(0)
            || BSP_EEPROM_WriteBuffer(0, (uint8_t *) in_config,
                                      CONFIG_BASE_ADDR, sizeof(*in_config))) {
        return -1;
    }
    return 0;
}

int config_load(config_t *out_config) {
    if (eeprom_init()) {
        return -1;
    }
    char magic[MAGIC_SIZE];
    uint16_t magic_size = MAGIC_SIZE;
    uint16_t config_size = sizeof(config_t);
    if (BSP_EEPROM_ReadBuffer(0, (uint8_t *) magic, MAGIC_BASE_ADDR,
                              magic_size)) {
        return -2;
    }
    if (BSP_EEPROM_IsDeviceReady(0)) {
        return -3;
    }
    if (strcmp(magic, MAGIC)) {
        return -4;
    }
    if (BSP_EEPROM_ReadBuffer(0, (uint8_t *) out_config, CONFIG_BASE_ADDR,
                              config_size)) {
        return -5;
    }
    if (BSP_EEPROM_IsDeviceReady(0)) {
        return -6;
    }

    return 0;
}
