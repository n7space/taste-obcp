#include "obcp_engine_thread_local.h"

/* General assumptions:
   This is supporting code for obcp_engine.c, providing Thread Local Storage on the following platforms:
   - Linux,
   - RTEMS 6,
   - FreeRTOS (generic).
   The target HW is x86-64 and ARM Cortex-M.
   For RTEMS 6, non-public API is used as POSIX API is not available in ESA's SMP QDP version.

   The code is used only by obcp_engine.c, and thus the following assumptions hold true:
   - tls_init() is called and finished before any tls_bind()

*/

_Static_assert((int)obcp_thread_local_value_index_max > 0,
               "obcp_thread_local_value_index_max must be at least 1");

#if defined(GENERIC_LINUX_TARGET) || defined(__linux__)

#include <assert.h>

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
   assert(index < obcp_thread_local_value_index_max);
   return tls_values[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   assert(index < obcp_thread_local_value_index_max);
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
#include <rtems/score/thread.h>
#include <rtems/score/percpu.h>
#include <rtems/score/threadimpl.h>
#include <stdatomic.h>
#include <string.h>

/* Compile-time guard: the TCB argument field is uint32_t; uintptr_t must be
 * the same width on this target or the pool pointer will be silently truncated. */
_Static_assert(sizeof(uintptr_t) == sizeof(uint32_t),
               "uintptr_t must be 32 bits; the RTEMS TCB argument field is uint32_t");

/* Internal storage pool, zero-initialised at program start. */
static uintptr_t tls_pool[OBCP_ENGINE_TLS_MAX_THREADS][obcp_thread_local_value_index_max];
static atomic_uint tls_pool_used = 0;

/* Registry of TCBs that have been bound, used by tls_init() to clear them on re-init. */
static Thread_Control *tls_bound_tasks[OBCP_ENGINE_TLS_MAX_THREADS];

static inline uintptr_t *tls_current(void)
{
   uintptr_t *ptr = (uintptr_t *)(uintptr_t)
         _Thread_Get_executing()->Start.Entry.Kinds.Numeric.argument;
   if (ptr == NULL)
   {
      /* tls_bind() was never called or the pool was exhausted — unrecoverable. */
      rtems_fatal(RTEMS_FATAL_SOURCE_APPLICATION, 0u);
   }
   return ptr;
}

void obcp_engine_tls_init(void)
{
   uint32_t i;
   /* Clear the TCB slot of every previously bound task so that tasks will
    * re-bind on their next do_work call, preventing slot re-use aliasing.
    * Caller must ensure no worker thread is concurrently in tls_bind(). */
   for (i = 0; i < OBCP_ENGINE_TLS_MAX_THREADS; i++)
   {
      if (tls_bound_tasks[i] != NULL)
      {
         tls_bound_tasks[i]->Start.Entry.Kinds.Numeric.argument = 0u;
         tls_bound_tasks[i] = NULL;
      }
   }
   /* Zero the pool so stale values are not visible to newly bound tasks. */
   memset(tls_pool, 0, sizeof(tls_pool));
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
      /* Pool exhausted — programming error, no recovery possible.
       * Roll back the counter so the overflow value does not permanently
       * consume a logical slot, then halt via the RTEMS fatal handler. */
      atomic_fetch_sub(&tls_pool_used, 1u);
      rtems_fatal(RTEMS_FATAL_SOURCE_APPLICATION, 1u);
   }

   /* Record this TCB in the registry so tls_init() can unregister it later. */
   tls_bound_tasks[slot] = _Thread_Get_executing();
   _Thread_Get_executing()->Start.Entry.Kinds.Numeric.argument =
         (uint32_t)(uintptr_t)tls_pool[slot];
}

uintptr_t obcp_engine_tls_get(uint32_t index)
{
   if (index >= obcp_thread_local_value_index_max)
   {
      rtems_fatal(RTEMS_FATAL_SOURCE_APPLICATION, 2u);
   }
   return tls_current()[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   if (index >= obcp_thread_local_value_index_max)
   {
      rtems_fatal(RTEMS_FATAL_SOURCE_APPLICATION, 2u);
   }
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
#elif defined(GENERIC_FREERTOS_TARGET)

#include "FreeRTOS.h"
#include "task.h"
#include <stdatomic.h>
#include <string.h>

#ifndef OBCP_FREERTOS_TLS_SLOT_INDEX
#define OBCP_FREERTOS_TLS_SLOT_INDEX (0)
#endif

#if configNUM_THREAD_LOCAL_STORAGE_POINTERS <= OBCP_FREERTOS_TLS_SLOT_INDEX
#error "FreeRTOSConfig.h must define configNUM_THREAD_LOCAL_STORAGE_POINTERS > OBCP_FREERTOS_TLS_SLOT_INDEX"
#endif

/* Internal storage pool, zero-initialised at program start. */
static uintptr_t tls_pool[OBCP_ENGINE_TLS_MAX_THREADS][obcp_thread_local_value_index_max];
static atomic_uint tls_pool_used = 0;

/* Registry of task handles that have been bound, used by tls_init() to clear them on re-init. */
static TaskHandle_t tls_bound_tasks[OBCP_ENGINE_TLS_MAX_THREADS];

static inline uintptr_t *tls_current(void)
{
   uintptr_t *ptr = (uintptr_t *)pvTaskGetThreadLocalStoragePointer(
         NULL, OBCP_FREERTOS_TLS_SLOT_INDEX);
   if (ptr == NULL)
   {
      /* tls_bind() was never called or the pool was exhausted — unrecoverable. */
      configASSERT(ptr != NULL);
      for (;;) {} /* unreachable safety net if configASSERT is a no-op in release */
   }
   return ptr;
}

void obcp_engine_tls_init(void)
{
   uint32_t i;
   /* Clear the TLS slot of every previously bound task so that tasks will
    * re-bind on their next do_work call, preventing slot re-use aliasing.
    * Caller must ensure no worker thread is concurrently in tls_bind(). */
   for (i = 0; i < OBCP_ENGINE_TLS_MAX_THREADS; i++)
   {
      if (tls_bound_tasks[i] != NULL)
      {
         vTaskSetThreadLocalStoragePointer(tls_bound_tasks[i],
                                          OBCP_FREERTOS_TLS_SLOT_INDEX, NULL);
         tls_bound_tasks[i] = NULL;
      }
   }
   /* Zero the pool so stale values are not visible to newly bound tasks. */
   memset(tls_pool, 0, sizeof(tls_pool));
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
      /* Pool exhausted — programming error, no recovery possible.
       * Roll back the counter so the overflow value does not permanently
       * consume a logical slot, then halt via the FreeRTOS fatal hook. */
      atomic_fetch_sub(&tls_pool_used, 1u);
      configASSERT(slot < OBCP_ENGINE_TLS_MAX_THREADS);
      for (;;) {} /* unreachable safety net if configASSERT is a no-op in release */
   }

   /* Record this task handle in the registry so tls_init() can unregister it later. */
   tls_bound_tasks[slot] = xTaskGetCurrentTaskHandle();
   vTaskSetThreadLocalStoragePointer(NULL, OBCP_FREERTOS_TLS_SLOT_INDEX,
                                     (void *)tls_pool[slot]);
}

uintptr_t obcp_engine_tls_get(uint32_t index)
{
   configASSERT(index < obcp_thread_local_value_index_max);
   if (index >= obcp_thread_local_value_index_max)
   {
      for (;;) {} /* unreachable safety net if configASSERT is a no-op in release */
   }
   return tls_current()[index];
}

void obcp_engine_tls_set(uint32_t index, uintptr_t value)
{
   configASSERT(index < obcp_thread_local_value_index_max);
   if (index >= obcp_thread_local_value_index_max)
   {
      for (;;) {} /* unreachable safety net if configASSERT is a no-op in release */
   }
   tls_current()[index] = value;
}

#else
#error "Unsupported target: define GENERIC_LINUX_TARGET, RTEMS_5, RTEMS_6, or GENERIC_FREERTOS_TARGET"
#endif /* platform selection */
