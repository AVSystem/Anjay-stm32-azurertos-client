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

#ifndef BSP_SENSOR_DRIVERS_H
#define BSP_SENSOR_DRIVERS_H

#include "basic_sensor_driver.h"
#include "three_axis_sensor_driver.h"

#ifdef THERMOMETER_AVAILABLE
extern const basic_sensor_driver_t BSP_THERMOMETER_DRIVER;
#endif // THERMOMETER_AVAILABLE
#ifdef HYGROMETER_AVAILABLE
extern const basic_sensor_driver_t BSP_HYGROMETER_DRIVER;
#endif // HYGROMETER_AVAILABLE
#ifdef BAROMETER_AVAILABLE
extern const basic_sensor_driver_t BSP_BAROMETER_DRIVER;
#endif // BAROMETER_AVAILABLE

#ifdef ACCELEROMETER_AVAILABLE
extern const three_axis_sensor_driver_t BSP_ACCELEROMETER_DRIVER;
#endif // ACCELEROMETER_AVAILABLE
#ifdef MAGNETOMETER_AVAILABLE
extern const three_axis_sensor_driver_t BSP_MAGNETOMETER_DRIVER;
#endif // MAGNETOMETER_AVAILABLE

#endif // BSP_SENSOR_DRIVERS_H
