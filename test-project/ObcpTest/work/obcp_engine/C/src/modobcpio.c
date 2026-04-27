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

// write(text): write a text string to the output
static mp_obj_t obcpio_write(mp_obj_t text_obj)
{
    if (obcp_engine_context.obcp_write == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_write is not provided by the runtime"));
    }
    size_t text_len;
    const char *text = mp_obj_str_get_data(text_obj, &text_len);

    if (!obcp_engine_context.obcp_write(text, text_len))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("write failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpio_write_obj, obcpio_write);

static const mp_rom_map_elem_t obcpio_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcpio)},
    {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&obcpio_write_obj)},
};

static MP_DEFINE_CONST_DICT(obcpio_module_globals, obcpio_module_globals_table);

const mp_obj_module_t mp_module_obcpio = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcpio_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcpio, mp_module_obcpio);
