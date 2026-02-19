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

// readintparameter(id): read an integer parameter with the given ID
static mp_obj_t obcpdatapool_readintparameter(mp_obj_t id_obj)
{
    if (obcp_engine_context.obcp_read_int_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_read_int_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    int32_t value;
    if (!obcp_engine_context.obcp_read_int_parameter(id, &value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_obj_new_int(value);
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpdatapool_readintparameter_obj, obcpdatapool_readintparameter);

// writeintparameter(id, value): write an integer parameter with the given ID
static mp_obj_t obcpdatapool_writeintparameter(mp_obj_t id_obj, mp_obj_t value_obj)
{
    if (obcp_engine_context.obcp_write_int_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_write_int_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    mp_int_t value = mp_obj_get_int(value_obj);
    if (!obcp_engine_context.obcp_write_int_parameter(id, value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcpdatapool_writeintparameter_obj, obcpdatapool_writeintparameter);

// readfloatparameter(id): read a float parameter with the given ID
static mp_obj_t obcpdatapool_readfloatparameter(mp_obj_t id_obj)
{
    if (obcp_engine_context.obcp_read_float_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_read_float_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    float value;
    if (!obcp_engine_context.obcp_read_float_parameter(id, &value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_obj_new_float(value);
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpdatapool_readfloatparameter_obj, obcpdatapool_readfloatparameter);

// writefloatparameter(id, value): write a float parameter with the given ID
static mp_obj_t obcpdatapool_writefloatparameter(mp_obj_t id_obj, mp_obj_t value_obj)
{
    if (obcp_engine_context.obcp_write_float_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_write_float_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    mp_float_t value = mp_obj_get_float(value_obj);
    if (!obcp_engine_context.obcp_write_float_parameter(id, value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcpdatapool_writefloatparameter_obj, obcpdatapool_writefloatparameter);

// readboolparameter(id): read a boolean parameter with the given ID
static mp_obj_t obcpdatapool_readboolparameter(mp_obj_t id_obj)
{
    if (obcp_engine_context.obcp_read_bool_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_read_bool_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    bool value;
    if (!obcp_engine_context.obcp_read_bool_parameter(id, &value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_obj_new_bool(value);
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpdatapool_readboolparameter_obj, obcpdatapool_readboolparameter);

// writeboolparameter(id, value): write a boolean parameter with the given ID
static mp_obj_t obcpdatapool_writeboolparameter(mp_obj_t id_obj, mp_obj_t value_obj)
{
    if (obcp_engine_context.obcp_write_bool_parameter == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_write_bool_parameter is not provided by the runtime"));
    }
    mp_uint_t id = mp_obj_get_uint(id_obj);
    bool value = mp_obj_is_true(value_obj);
    if (!obcp_engine_context.obcp_write_bool_parameter(id, value))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("datapool access failed"));
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcpdatapool_writeboolparameter_obj, obcpdatapool_writeboolparameter);

static const mp_rom_map_elem_t obcpdatapool_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcpdatapool)},
    {MP_ROM_QSTR(MP_QSTR_readintparameter), MP_ROM_PTR(&obcpdatapool_readintparameter_obj)},
    {MP_ROM_QSTR(MP_QSTR_writeintparameter), MP_ROM_PTR(&obcpdatapool_writeintparameter_obj)},
    {MP_ROM_QSTR(MP_QSTR_readfloatparameter), MP_ROM_PTR(&obcpdatapool_readfloatparameter_obj)},
    {MP_ROM_QSTR(MP_QSTR_writefloatparameter), MP_ROM_PTR(&obcpdatapool_writefloatparameter_obj)},
    {MP_ROM_QSTR(MP_QSTR_readboolparameter), MP_ROM_PTR(&obcpdatapool_readboolparameter_obj)},
    {MP_ROM_QSTR(MP_QSTR_writeboolparameter), MP_ROM_PTR(&obcpdatapool_writeboolparameter_obj)},
};

static MP_DEFINE_CONST_DICT(obcpdatapool_module_globals, obcpdatapool_module_globals_table);

const mp_obj_module_t mp_module_obcpdatapool = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcpdatapool_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcpdatapool, mp_module_obcpdatapool);
