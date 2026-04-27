/*
 * This file is part of N7S CFDP project, https://github.com/n7space/n7s-obcp
 * The MIT License (MIT)
 * Copyright (c) 2026 N7 Space sp. z o.o.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2026
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "obj.h"
#include "runtime.h"
#include "obcpengine.h"

// Not exposed in OBCP Engine header
extern obcp_engine_context_t obcp_engine_context;

// beginstep(id): begin a control step with the given ID
static mp_obj_t obcpcontrol_beginstep(mp_obj_t id_obj)
{
    if (obcp_engine_context.obcp_beginstep == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_beginstep is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);

    if (!obcp_engine_context.obcp_beginstep(id))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_beginstep failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpcontrol_beginstep_obj, obcpcontrol_beginstep);

// endstep(id, success): end a control step with the given ID and success status
static mp_obj_t obcpcontrol_endstep(mp_obj_t id_obj, mp_obj_t success_obj)
{
    if (obcp_engine_context.obcp_endstep == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_endstep is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    bool success = mp_obj_is_true(success_obj);

    if (!obcp_engine_context.obcp_endstep(id, success))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_endstep failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcpcontrol_endstep_obj, obcpcontrol_endstep);

static const mp_rom_map_elem_t obcpcontrol_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcpcontrol)},
    {MP_ROM_QSTR(MP_QSTR_beginstep), MP_ROM_PTR(&obcpcontrol_beginstep_obj)},
    {MP_ROM_QSTR(MP_QSTR_endstep), MP_ROM_PTR(&obcpcontrol_endstep_obj)},
};

static MP_DEFINE_CONST_DICT(obcpcontrol_module_globals, obcpcontrol_module_globals_table);

const mp_obj_module_t mp_module_obcpcontrol = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcpcontrol_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcpcontrol, mp_module_obcpcontrol);
