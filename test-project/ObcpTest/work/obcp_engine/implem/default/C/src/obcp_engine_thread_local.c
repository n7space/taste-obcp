#include "obcp_engine_thread_local.h"

#if defined(GENERIC_LINUX_TARGET) || defined(__linux__)

static __thread uintptr_t tls_values[obcp_thread_local_value_index_max];

void obcp_engine_tls_init(void)
{
   /* __thread variables are zero-initialised per-thread by the runtime. */
}

void obcp_engine_tls_bind(void)
{
   /* No-op: __thread already provides per-thread storage. */
}

uintptr_t obcp_engine_tls_get(uint32_t index)
{
   return tls_values[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   tls_values[index] = value;
}

/* =========================================================================
 * RTEMS 5 / 6  (Classic API — POSIX not available on QDP targets)
 *
 * Storage pool
 * ------------
 * A static pool of per-thread value arrays is allocated from
 * tls_pool[OBCP_ENGINE_TLS_MAX_THREADS].  tls_bind() is idempotent: on the
 * first call from a given task it atomically claims the next free pool slot
 * and stores its address in the TCB field
 * Start.Entry.Kinds.Numeric.argument (unused by RTEMS after task start).
 * Subsequent calls from the same task see a non-zero field and return early.
 * ========================================================================= */
#elif defined(RTEMS_5) || defined(RTEMS_6)

#include <rtems.h>
#include <stdatomic.h>

/* Internal storage pool, zero-initialised at program start. */
static uintptr_t tls_pool[OBCP_ENGINE_TLS_MAX_THREADS][obcp_thread_local_value_index_max];
static atomic_uint tls_pool_used = 0;

static inline uintptr_t *tls_current(void)
{
   return (uintptr_t *)(uintptr_t)
         _Thread_Get_executing()->Start.Entry.Kinds.Numeric.argument;
}

void obcp_engine_tls_init(void)
{
   /* Reset pool counter — workers will re-bind on their next do_work call. */
   tls_pool_used = 0;
}

void obcp_engine_tls_bind(void)
{
   /* Idempotent: if the TCB slot is already set this thread is already bound. */
   if (_Thread_Get_executing()->Start.Entry.Kinds.Numeric.argument != 0)
   {
      return;
   }

   /* Atomically grab the next slot from the pool. */
   uint32_t slot = atomic_fetch_add(&tls_pool_used, 1u);
   if (slot >= OBCP_ENGINE_TLS_MAX_THREADS)
   {
      /* Pool exhausted — programming error, no recovery possible. */
      return;
   }

   _Thread_Get_executing()->Start.Entry.Kinds.Numeric.argument =
         (uint32_t)(uintptr_t)tls_pool[slot];
}

uintptr_t obcp_engine_tls_get(uint32_t index)
{
   return tls_current()[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   tls_current()[index] = value;
}

/* =========================================================================
 * FreeRTOS
 *
 * A FreeRTOS task-local storage pointer slot (OBCP_FREERTOS_TLS_SLOT_INDEX)
 * is used to anchor a pool entry for the current task.  tls_bind() is
 * idempotent: if the slot is already non-NULL the task is already bound.
 *
 * Requires configNUM_THREAD_LOCAL_STORAGE_POINTERS > OBCP_FREERTOS_TLS_SLOT_INDEX
 * in FreeRTOSConfig.h.
 * ========================================================================= */
#else

#include "FreeRTOS.h"
#include "task.h"
#include <stdatomic.h>

#ifndef OBCP_FREERTOS_TLS_SLOT_INDEX
#define OBCP_FREERTOS_TLS_SLOT_INDEX (0)
#endif

/* Internal storage pool, zero-initialised at program start. */
static uintptr_t tls_pool[OBCP_ENGINE_TLS_MAX_THREADS][obcp_thread_local_value_index_max];
static atomic_uint tls_pool_used = 0;

static inline uintptr_t *tls_current(void)
{
   return (uintptr_t *)pvTaskGetThreadLocalStoragePointer(
         NULL, OBCP_FREERTOS_TLS_SLOT_INDEX);
}

void obcp_engine_tls_init(void)
{
   /* Reset pool counter — workers will re-bind on their next do_work call. */
   tls_pool_used = 0;
}

void obcp_engine_tls_bind(void)
{
   /* Idempotent: if the task-local slot is already set the task is already bound. */
   if (pvTaskGetThreadLocalStoragePointer(NULL, OBCP_FREERTOS_TLS_SLOT_INDEX) != NULL)
   {
      return;
   }

   /* Atomically grab the next slot from the pool. */
   uint32_t slot = atomic_fetch_add(&tls_pool_used, 1u);
   if (slot >= OBCP_ENGINE_TLS_MAX_THREADS)
   {
      return;
   }

   vTaskSetThreadLocalStoragePointer(NULL, OBCP_FREERTOS_TLS_SLOT_INDEX,
                                     (void *)tls_pool[slot]);
}

uintptr_t obcp_engine_tls_get(uint32_t index)
{
   return tls_current()[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   tls_current()[index] = value;
}

#endif /* platform selection */
