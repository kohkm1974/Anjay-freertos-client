/*
 * Copyright 2020 AVSystem <avsystem@avsystem.com>
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

#include <anjay/anjay.h>

#include "basic_sensor_object.h"
#include "sensor_drivers/basic_sensor_driver.h"
#include "sensor_drivers/iks01a2_sensor_drivers.h"

static basic_sensor_object_t g_humidity_obj;

int humidity_object_install(anjay_t *anjay) {
    return basic_sensor_object_install(anjay, &IKS01A2_HYGROMETER_DRIVER, 3304,
                                       &g_humidity_obj);
}

void humidity_object_update(anjay_t *anjay) {
    basic_sensor_object_update(anjay, &g_humidity_obj);
}
