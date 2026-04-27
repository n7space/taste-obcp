#ifndef OBCP_ENGINE_THREAD_LOCAL_H
#define OBCP_ENGINE_THREAD_LOCAL_H

#include <stdint.h>
#include "obcpengine.h" 

/* Maximum number of concurrent worker threads supported by the TLS pool.
 * Override by defining OBCP_ENGINE_TLS_MAX_THREADS before including this header. */
#ifndef OBCP_ENGINE_TLS_MAX_THREADS
#define OBCP_ENGINE_TLS_MAX_THREADS (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** One-time engine init.  Resets all internal state.  Call from obcp_engine_startup. */
void obcp_engine_tls_init(void);

/** Bind the calling thread to a TLS slot.  Idempotent — safe to call on every
 *  do_work entry.  Must be called from the worker thread itself. */
void obcp_engine_tls_bind(void);

/** Get the TLS value at 'index'. Signature matches obcp_engine_get_thread_local_value_t. */
uintptr_t obcp_engine_tls_get(uint32_t index);

/** Set the TLS value at 'index' to 'value'. Signature matches obcp_engine_set_thread_local_value_t. */
void obcp_engine_tls_set(uint32_t index, uintptr_t value);

#ifdef __cplusplus
}
#endif

#endif /* OBCP_ENGINE_THREAD_LOCAL_H */
