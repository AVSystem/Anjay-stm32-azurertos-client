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

#include <stdint.h>

#include <cmsis_os.h>
#include <tx_port.h>

#include <avsystem/commons/avs_time.h>

avs_time_monotonic_t avs_time_monotonic_now(void) {
    static int64_t prev_tick = 0;
    static int64_t base_tick = 0;

    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE

    int64_t ticks = (int64_t) osKernelGetTickCount();

    if (ticks < prev_tick) {
        base_tick += (int64_t) UINT32_MAX + 1;
    }
    prev_tick = ticks;
    ticks += base_tick;

    TX_RESTORE

    const int64_t time_ms = ticks * 1000 / osKernelGetTickFreq();
    return avs_time_monotonic_from_scalar(time_ms, AVS_TIME_MS);
}

avs_time_real_t avs_time_real_now(void) {
    avs_time_real_t result = {
        .since_real_epoch = avs_time_monotonic_now().since_monotonic_epoch
    };
    return result;
}
