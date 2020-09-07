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

/**
 * Generated by anjay_codegen.py on 2020-07-20 13:01:23
 *
 * LwM2M Object: Multiple Axis Joystick
 * ID: 3345, URN: urn:oma:lwm2m:ext:3345, Optional, Multiple
 *
 * This IPSO object can be used to report the position of a shuttle or
 * joystick control. A digital input is provided to report the state of
 * an associated push button.
 */
#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>

#include <anjay/anjay.h>
#include <avsystem/commons/avs_defs.h>
#include <avsystem/commons/avs_list.h>
#include <avsystem/commons/avs_log.h>
#include <avsystem/commons/avs_memory.h>

#include "stm32l496g_discovery.h"

#define JOYSTICK_OBJ_LOG(...) avs_log(joystick_obj, __VA_ARGS__)

/**
 * Digital Input State: R, Single, Optional
 * type: boolean, range: N/A, unit: N/A
 * The current state of a digital input.
 */
#define RID_DIGITAL_INPUT_STATE 5500

/**
 * Digital Input Counter: R, Single, Optional
 * type: integer, range: N/A, unit: N/A
 * The cumulative value of active state detected.
 */
#define RID_DIGITAL_INPUT_COUNTER 5501

/**
 * X Value: R, Single, Optional
 * type: float, range: N/A, unit: N/A
 * The measured value along the X axis.
 */
#define RID_X_VALUE 5702

/**
 * Y Value: R, Single, Optional
 * type: float, range: N/A, unit: N/A
 * The measured value along the Y axis.
 */
#define RID_Y_VALUE 5703

#define JOYSTICK_OID 3345

typedef struct multiple_axis_joystick_object_struct {
    const anjay_dm_object_def_t *def;

    bool sel;
    atomic_uint_fast32_t sel_counter;
    uint32_t last_sel_counter;
    int8_t x_value;
    int8_t y_value;
} multiple_axis_joystick_object_t;

static inline multiple_axis_joystick_object_t *
get_obj(const anjay_dm_object_def_t *const *obj_ptr) {
    assert(obj_ptr);
    return AVS_CONTAINER_OF(obj_ptr, multiple_axis_joystick_object_t, def);
}

static int list_instances(anjay_t *anjay,
                          const anjay_dm_object_def_t *const *obj_ptr,
                          anjay_dm_list_ctx_t *ctx) {
    (void) anjay;
    (void) obj_ptr;

    anjay_dm_emit(ctx, 0);
    return 0;
}

static int instance_reset(anjay_t *anjay,
                          const anjay_dm_object_def_t *const *obj_ptr,
                          anjay_iid_t iid) {
    (void) anjay;
    assert(iid == 0);
    (void) iid;

    multiple_axis_joystick_object_t *obj = get_obj(obj_ptr);
    atomic_store(&obj->sel_counter, 0);
    return 0;
}

static int list_resources(anjay_t *anjay,
                          const anjay_dm_object_def_t *const *obj_ptr,
                          anjay_iid_t iid,
                          anjay_dm_resource_list_ctx_t *ctx) {
    (void) anjay;
    (void) obj_ptr;
    (void) iid;

    anjay_dm_emit_res(ctx, RID_DIGITAL_INPUT_STATE, ANJAY_DM_RES_R,
                      ANJAY_DM_RES_PRESENT);
    anjay_dm_emit_res(ctx, RID_DIGITAL_INPUT_COUNTER, ANJAY_DM_RES_R,
                      ANJAY_DM_RES_PRESENT);
    anjay_dm_emit_res(ctx, RID_X_VALUE, ANJAY_DM_RES_R, ANJAY_DM_RES_PRESENT);
    anjay_dm_emit_res(ctx, RID_Y_VALUE, ANJAY_DM_RES_R, ANJAY_DM_RES_PRESENT);
    return 0;
}

static int8_t read_x_value() {
    if (HAL_GPIO_ReadPin(RIGHT_JOY_GPIO_PORT, RIGHT_JOY_PIN)) {
        return 1;
    }
    if (HAL_GPIO_ReadPin(LEFT_JOY_GPIO_PORT, LEFT_JOY_PIN)) {
        return -1;
    }
    return 0;
}

static int8_t read_y_value() {
    if (HAL_GPIO_ReadPin(UP_JOY_GPIO_PORT, UP_JOY_PIN)) {
        return 1;
    }
    if (HAL_GPIO_ReadPin(DOWN_JOY_GPIO_PORT, DOWN_JOY_PIN)) {
        return -1;
    }
    return 0;
}

static bool read_sel() {
    return HAL_GPIO_ReadPin(SEL_JOY_GPIO_PORT, SEL_JOY_PIN);
}

static int resource_read(anjay_t *anjay,
                         const anjay_dm_object_def_t *const *obj_ptr,
                         anjay_iid_t iid,
                         anjay_rid_t rid,
                         anjay_riid_t riid,
                         anjay_output_ctx_t *ctx) {
    (void) anjay;

    multiple_axis_joystick_object_t *obj = get_obj(obj_ptr);
    assert(iid == 0);
    (void) iid;
    assert(obj);

    switch (rid) {
    case RID_DIGITAL_INPUT_STATE:
        assert(riid == ANJAY_ID_INVALID);
        return anjay_ret_bool(ctx, read_sel());

    case RID_DIGITAL_INPUT_COUNTER:
        assert(riid == ANJAY_ID_INVALID);
        return anjay_ret_i32(ctx, atomic_load(&obj->sel_counter));

    case RID_X_VALUE:
        assert(riid == ANJAY_ID_INVALID);
        return anjay_ret_float(ctx, read_x_value());

    case RID_Y_VALUE:
        assert(riid == ANJAY_ID_INVALID);
        return anjay_ret_float(ctx, read_y_value());

    default:
        return ANJAY_ERR_METHOD_NOT_ALLOWED;
    }
}

static const anjay_dm_object_def_t OBJ_DEF = {
    .oid = JOYSTICK_OID,
    .handlers = {
        .list_instances = list_instances,
        .instance_reset = instance_reset,

        .list_resources = list_resources,
        .resource_read = resource_read,
    }
};

static multiple_axis_joystick_object_t JOY_OBJECT = {
    .def = &OBJ_DEF
};

static const anjay_dm_object_def_t **OBJ_DEF_PTR = &JOY_OBJECT.def;

int joystick_object_install(anjay_t *anjay) {
    return anjay_register_object(anjay, OBJ_DEF_PTR);
}

void joystick_object_increment_counter(void) {
    multiple_axis_joystick_object_t *obj = get_obj(OBJ_DEF_PTR);
    atomic_fetch_add(&obj->sel_counter, 1);
}

void joystick_object_update(anjay_t *anjay) {
    multiple_axis_joystick_object_t *obj = get_obj(OBJ_DEF_PTR);

    int8_t x_value = read_x_value();
    int8_t y_value = read_y_value();
    bool sel = read_sel();

    if (obj->x_value != x_value) {
        obj->x_value = x_value;
        anjay_notify_changed(anjay, JOYSTICK_OID, 0, RID_X_VALUE);
    }

    if (obj->y_value != y_value) {
        obj->y_value = y_value;
        anjay_notify_changed(anjay, JOYSTICK_OID, 0, RID_Y_VALUE);
    }

    if (obj->sel != sel) {
        obj->sel = sel;
        anjay_notify_changed(anjay, JOYSTICK_OID, 0, RID_DIGITAL_INPUT_STATE);
    }

    uint32_t sel_counter_val = atomic_load(&obj->sel_counter);
    if (obj->last_sel_counter != sel_counter_val) {
        obj->last_sel_counter = sel_counter_val;
        anjay_notify_changed(anjay, JOYSTICK_OID, 0, RID_DIGITAL_INPUT_COUNTER);
    }
}