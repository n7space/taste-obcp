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

// sendevent(event_id): send an event with the given integer ID
static mp_obj_t obcpevents_sendevent(mp_obj_t event_id_obj)
{
    if (obcp_engine_context.obcp_send_event == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_send_event is not provided by the runtime"));
    }
    mp_uint_t event_id = mp_obj_get_uint(event_id_obj);

    obcp_engine_context.obcp_send_event(event_id);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcpevents_sendevent_obj, obcpevents_sendevent);

static const mp_rom_map_elem_t obcpevents_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcpevents)},
    {MP_ROM_QSTR(MP_QSTR_sendevent), MP_ROM_PTR(&obcpevents_sendevent_obj)},
};

static MP_DEFINE_CONST_DICT(obcpevents_module_globals, obcpevents_module_globals_table);

const mp_obj_module_t mp_module_obcpevents = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcpevents_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcpevents, mp_module_obcpevents);
