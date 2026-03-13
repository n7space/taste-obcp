#ifndef OBCP_ENGINE_H
#define OBCP_ENGINE_H

#include <stdint.h>
#include <stddef.h>

#include "obcpdatapool.h"
#include "obcpevents.h"
#include "obcpio.h"
#include "obcpcontrol.h"
#include "obcptime.h"
#include "obcppackets.h"

/* IDs of thread-local data items */
typedef enum {
  obcp_thread_local_value_index_mp_state = 0,
  obcp_thread_local_value_index_buffer = 1,
  obcp_thread_local_value_index_buffer_length = 2,
  obcp_thread_local_value_index_max
} obcp_thread_local_value_index;

/* Thread local data access via functions, to facilitate mocking and compatibility */
typedef uintptr_t (*obcp_engine_get_thread_local_value_t)(uint32_t index);
typedef void (*obcp_engine_set_thread_local_value_t)(uint32_t index, uintptr_t value);

/* Global engine context */
typedef struct {
    /* thread-local data access */
    obcp_engine_get_thread_local_value_t obcp_engine_get_thread_local_value;
    obcp_engine_set_thread_local_value_t obcp_engine_set_thread_local_value;
    /* datapool */
    obcp_read_int_parameter_t obcp_read_int_parameter;
    obcp_write_int_parameter_t obcp_write_int_parameter;
    obcp_read_enum_parameter_t obcp_read_enum_parameter;
    obcp_write_enum_parameter_t obcp_write_enum_parameter;
    obcp_read_float_parameter_t obcp_read_float_parameter;
    obcp_write_float_parameter_t obcp_write_float_parameter;
    obcp_read_bool_parameter_t obcp_read_bool_parameter;
    obcp_write_bool_parameter_t obcp_write_bool_parameter;
    /* events */
    obcp_send_event_t obcp_send_event;
    /* io */
    obcp_write_t obcp_write;
    /* control */
    obcp_beginstep_t obcp_beginstep;
    obcp_endstep_t obcp_endstep;
    /* time */
    obcp_wait_t obcp_wait;
    obcp_waituntil_t obcp_waituntil;
    obcp_gettime_t obcp_gettime;
    /* packets */
    obcp_is_packet_available_t obcp_is_packet_available;
    obcp_get_channel_with_packet_available_t obcp_get_channel_with_packet_available;
    obcp_can_send_packet_t obcp_can_send_packet;
    obcp_send_packet_t obcp_send_packet;
    obcp_receive_packet_t obcp_receive_packet;
} obcp_engine_context_t;

/**
 * @brief Initialize the OBCP engine.
 *
 * Sets up internal engine state and installs the provided callback
 * implementations from the supplied @p context. The context must remain
 * valid for the lifetime of the engine.
 *
 * @param context Pointer to an initialized #obcp_engine_context_t providing
 *                function callbacks used by the engine. Must not be NULL.
 * @return true on successful initialization, false on failure.
 */
bool obcpengine_init(obcp_engine_context_t* context);

/**
 * @brief Execute a Python source script.
 *
 * Executes a null-terminated Python source string in the embedded
 * MicroPython runtime backed by the provided heap memory.
 *
 * @param script Null-terminated string containing Python source code to run.
 * @param heap Pointer to a memory area to be used as the MicroPython heap.
 * @param heap_size Size, in bytes, of the heap buffer pointed to by @p heap.
 * @return true if the script executed successfully, false on error.
 */
bool obcpengine_execute_py(const char* const script, char* const heap, const size_t heap_size);

/**
 * @brief Execute a precompiled MicroPython (.mpy) script.
 *
 * Runs the provided compiled bytecode buffer inside the embedded
 * MicroPython runtime using the supplied heap.
 *
 * @param script Pointer to the .mpy bytecode buffer.
 * @param script_length Length, in bytes, of the @p script buffer.
 * @param heap Pointer to a memory area to be used as the MicroPython heap.
 * @param heap_size Size, in bytes, of the heap buffer pointed to by @p heap.
 * @return true if the bytecode executed successfully, false on error.
 */
bool obcpengine_execute_mpy(const uint8_t* const script, const size_t script_length, char* const heap, const size_t heap_size);

/**
 * @brief Provide a buffer for engine use.
 *
 * Supplies the engine with a buffer and its length. 
 * Buffer is used for packet related operations, and therefore:
 * - must be set if obcppackets module is used
 * - must be large enough to fit the largest possible packet
 * For non-concurrent engine, only one buffer is needed.
 * For concurrent engine, the buffer address is stored in thread-local
 * storage, and therefore a separate buffer must be supplied for each task
 * that executes micropython code.
 * 
 * @param buffer Pointer to the buffer being provided to the engine.
 * @param length Length, in bytes, of the provided buffer.
 * @return true if the buffer was accepted, false otherwise.
 */
bool obcpengine_provide_buffer(uint8_t* const buffer, const uint32_t length);

#endif