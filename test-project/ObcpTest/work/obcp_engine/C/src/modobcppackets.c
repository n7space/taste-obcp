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

// ispacketavailable(channel): check if a packet is available on the given channel
static mp_obj_t obcppackets_ispacketavailable(mp_obj_t channel_obj)
{
    if (obcp_engine_context.obcp_is_packet_available == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_is_packet_available is not provided by the runtime"));
    }
    mp_uint_t channel = mp_obj_get_uint(channel_obj);
    bool available = obcp_engine_context.obcp_is_packet_available(channel);
    return mp_obj_new_bool(available);
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcppackets_ispacketavailable_obj, obcppackets_ispacketavailable);

// getchannelwithpacketavailable(): get the first channel with a packet available, or None if none available
static mp_obj_t obcppackets_getchannelwithpacketavailable(void)
{
    if (obcp_engine_context.obcp_get_channel_with_packet_available == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_get_channel_with_packet_available is not provided by the runtime"));
    }
    uint32_t channel = 0;
    if (!obcp_engine_context.obcp_get_channel_with_packet_available(&channel))
    {
        return mp_const_none;
    }
    return mp_obj_new_int_from_uint(channel);
}
static MP_DEFINE_CONST_FUN_OBJ_0(obcppackets_getchannelwithpacketavailable_obj, obcppackets_getchannelwithpacketavailable);

// cansendpacket(channel): check if a packet can be sent on the given channel
static mp_obj_t obcppackets_cansendpacket(mp_obj_t channel_obj)
{
    if (obcp_engine_context.obcp_can_send_packet == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_can_send_packet is not provided by the runtime"));
    }
    mp_uint_t channel = mp_obj_get_uint(channel_obj);
    bool can_send = obcp_engine_context.obcp_can_send_packet(channel);
    return mp_obj_new_bool(can_send);
}
static MP_DEFINE_CONST_FUN_OBJ_1(obcppackets_cansendpacket_obj, obcppackets_cansendpacket);

// sendpacket(channel, data): send a packet on the given channel
static mp_obj_t obcppackets_sendpacket(mp_obj_t channel_obj, mp_obj_t data_obj)
{
    if (obcp_engine_context.obcp_send_packet == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_send_packet is not provided by the runtime"));
    }
    mp_uint_t channel = mp_obj_get_uint(channel_obj);

    // Get buffer info from bytes-like object
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(data_obj, &bufinfo, MP_BUFFER_READ);

    if (!obcp_engine_context.obcp_send_packet(channel, bufinfo.len, (const char *)bufinfo.buf))
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("failed to send packet"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcppackets_sendpacket_obj, obcppackets_sendpacket);

// receivepacket(channel, timeout_ms): receive a packet from the given channel, or None if no packet received
static mp_obj_t obcppackets_receivepacket(mp_obj_t channel_obj, mp_obj_t timeout_obj)
{
    if (obcp_engine_context.obcp_receive_packet == NULL)
    {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("obcp_receive_packet is not provided by the runtime"));
    }
    mp_uint_t channel = mp_obj_get_uint(channel_obj);
    mp_uint_t timeout_ms = mp_obj_get_uint(timeout_obj);

    char* buffer = (char*)(obcp_engine_context.obcp_engine_get_thread_local_value(obcp_thread_local_value_index_buffer));
    uint32_t length = (uint32_t)(obcp_engine_context.obcp_engine_get_thread_local_value(obcp_thread_local_value_index_buffer_length));

    if (!obcp_engine_context.obcp_receive_packet(channel, &length, buffer, timeout_ms))
    {
        return mp_const_none;
    }

    // Return a bytes object with the received data
    return mp_obj_new_bytes((const byte *)buffer, length);
}
static MP_DEFINE_CONST_FUN_OBJ_2(obcppackets_receivepacket_obj, obcppackets_receivepacket);

static const mp_rom_map_elem_t obcppackets_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_obcppackets)},
    {MP_ROM_QSTR(MP_QSTR_ispacketavailable), MP_ROM_PTR(&obcppackets_ispacketavailable_obj)},
    {MP_ROM_QSTR(MP_QSTR_getchannelwithpacketavailable), MP_ROM_PTR(&obcppackets_getchannelwithpacketavailable_obj)},
    {MP_ROM_QSTR(MP_QSTR_cansendpacket), MP_ROM_PTR(&obcppackets_cansendpacket_obj)},
    {MP_ROM_QSTR(MP_QSTR_sendpacket), MP_ROM_PTR(&obcppackets_sendpacket_obj)},
    {MP_ROM_QSTR(MP_QSTR_receivepacket), MP_ROM_PTR(&obcppackets_receivepacket_obj)},
};

static MP_DEFINE_CONST_DICT(obcppackets_module_globals, obcppackets_module_globals_table);

const mp_obj_module_t mp_module_obcppackets = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&obcppackets_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_obcppackets, mp_module_obcppackets);
