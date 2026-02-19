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

// wait(milliseconds): wait for the specified number of milliseconds
static mp_obj_t obcptime_wait(mp_obj_t milliseconds_obj)
{
    if (obcp_engine_context.obcp_wait == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_wait is not provided by the runtime"));
    }
    mp_uint_t milliseconds = mp_obj_get_uint(milliseconds_obj);

    if (!obcp_engine_context.obcp_wait(milliseconds))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_wait failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcptime_wait_obj, obcptime_wait);

// waituntil(seconds, milliseconds): wait until the specified absolute time
static mp_obj_t obcptime_waituntil(mp_obj_t seconds_obj, mp_obj_t milliseconds_obj)
{
    if (obcp_engine_context.obcp_waituntil == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_waituntil is not provided by the runtime"));
    }
    mp_uint_t seconds = mp_obj_get_uint(seconds_obj);
    mp_uint_t milliseconds = mp_obj_get_uint(milliseconds_obj);

    if (!obcp_engine_context.obcp_waituntil(seconds, milliseconds))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_waituntil failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcptime_waituntil_obj, obcptime_waituntil);

// gettime(): get the current system time as a tuple (seconds, milliseconds)
static mp_obj_t obcptime_gettime(void)
{
    if (obcp_engine_context.obcp_gettime == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_gettime is not provided by the runtime"));
    }
    uint32_t seconds;
    uint32_t milliseconds;

    if (!obcp_engine_context.obcp_gettime(&seconds, &milliseconds))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_gettime failed"));
    }

    mp_obj_t tuple[2];
    tuple[0] = mp_obj_new_int_from_uint(seconds);
    tuple[1] = mp_obj_new_int_from_uint(milliseconds);

    return mp_obj_new_tuple(2, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_0(obcptime_gettime_obj, obcptime_gettime);

static const mp_rom_map_elem_t obcptime_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcptime)},
    {MP_ROM_QSTR(MP_QSTR_wait), MP_ROM_PTR(&obcptime_wait_obj)},
    {MP_ROM_QSTR(MP_QSTR_waituntil), MP_ROM_PTR(&obcptime_waituntil_obj)},
    {MP_ROM_QSTR(MP_QSTR_gettime), MP_ROM_PTR(&obcptime_gettime_obj)},
};

static MP_DEFINE_CONST_DICT(obcptime_module_globals, obcptime_module_globals_table);

const mp_obj_module_t mp_module_obcptime = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcptime_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcptime, mp_module_obcptime);
