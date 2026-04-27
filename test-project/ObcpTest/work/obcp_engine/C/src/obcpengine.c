#include <assert.h>
#include "obcpengine.h"
#include "mpstate.h"
#include "micropython_embed.h"

// Initial values, so that functions remain unempty
// This is intended to work in a single-threaded environment
static uintptr_t dummy_getter(uint32_t index);
static void dummy_setter(uint32_t index, uintptr_t value);

obcp_engine_context_t obcp_engine_context = {
    .obcp_beginstep = NULL,
    .obcp_can_send_packet = NULL,
    .obcp_endstep = NULL,
    .obcp_engine_get_thread_local_value = dummy_getter,
    .obcp_engine_set_thread_local_value = dummy_setter,
    .obcp_get_channel_with_packet_available = NULL,
    .obcp_gettime = NULL,
    .obcp_is_packet_available = NULL,
    .obcp_read_bool_parameter = NULL,
    .obcp_read_float_parameter = NULL,
    .obcp_read_int_parameter = NULL,
    .obcp_receive_packet = NULL,
    .obcp_send_event = NULL,
    .obcp_send_packet = NULL,
    .obcp_wait = NULL,
    .obcp_waituntil = NULL,
    .obcp_write = NULL,
    .obcp_write_bool_parameter = NULL,
    .obcp_write_float_parameter = NULL,
    .obcp_write_int_parameter = NULL,
};

uintptr_t dummy_storage[obcp_thread_local_value_index_max];

static uintptr_t dummy_getter(uint32_t index)
{
    return dummy_storage[index];
}

static void dummy_setter(uint32_t index, uintptr_t value)
{
    dummy_storage[index] = value;
}

/* Implementation of mp_state pointer accessor, used by the modified MicroPython runtime */
void* mp_state_ptr();

void* mp_state_ptr()
{
    return (void*)(obcp_engine_context.obcp_engine_get_thread_local_value(obcp_thread_local_value_index_mp_state));
}

bool obcpengine_init(obcp_engine_context_t* context)
{
    obcp_engine_context = *context;
#ifdef OBCP_ENABLE_CONCURRENT_OBCPS 
    assert(obcp_engine_context.obcp_engine_set_thread_local_value != NULL);
    assert(obcp_engine_context.obcp_engine_get_thread_local_value != NULL);
#else
    // Without concurrency, the setters and getters are still needed for getting/setting buffers
    obcp_engine_context.obcp_engine_set_thread_local_value = dummy_setter;
    obcp_engine_context.obcp_engine_get_thread_local_value = dummy_getter;
#endif
    return true;
}

bool obcpengine_execute_py(const char* const script, char* const heap, const size_t heap_size)
{
#ifdef OBCP_ENABLE_CONCURRENT_OBCPS
    mp_state_ctx_t mp_state_ctx;
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_mp_state, (uintptr_t)&mp_state_ctx);
#endif
    int stack_top;

    mp_embed_init(&heap[0], heap_size, &stack_top);
    mp_embed_exec_str(script);
    mp_embed_deinit();
#ifdef OBCP_ENABLE_CONCURRENT_OBCPS
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_mp_state, (uintptr_t)NULL);
#endif
    return true;
}

bool obcpengine_execute_mpy(const uint8_t* const script, const size_t script_length, char* const heap, const size_t heap_size)
{
#ifdef OBCP_ENABLE_CONCURRENT_OBCPS
    mp_state_ctx_t mp_state_ctx;
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_mp_state, (uintptr_t)&mp_state_ctx);
#endif
    int stack_top;

    mp_embed_init(&heap[0], heap_size, &stack_top);
    mp_embed_exec_mpy(script, script_length);
    mp_embed_deinit();

#ifdef OBCP_ENABLE_CONCURRENT_OBCPS
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_mp_state, (uintptr_t)NULL);
#endif
    return true;
}

bool obcpengine_provide_buffer(uint8_t* const buffer, const uint32_t length)
{
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_buffer, (uintptr_t)buffer);
    obcp_engine_context.obcp_engine_set_thread_local_value(obcp_thread_local_value_index_buffer_length, (uintptr_t)length);
    return true;
}