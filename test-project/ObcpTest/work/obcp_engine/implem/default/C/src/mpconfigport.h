/* This file is part of N7S CFDP project, https://github.com/n7space/n7s-obcp
 * The MIT License (MIT)
 * Copyright (c) 2026 N7 Space sp. z o.o.
 */

// Include common MicroPython embed configuration.
#include "mpconfigport_common.h"

// Use the minimal starting configuration (disables all optional features).
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)

// MicroPython configuration.
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PY_GC                           (1)
// To properly support uints used by events
#define MICROPY_LONGINT_IMPL (MICROPY_LONGINT_IMPL_LONGLONG)
// Enable loading/executing precompiled .mpy persistent code
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
// Enable float support (required for datapool)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_PY_BUILTINS_FLOAT               (1)
#define MICROPY_ERROR_REPORTING (MICROPY_ERROR_REPORTING_DETAILED)

#ifdef OBCP_ENABLE_CONCURRENT_OBCPS
// Enable support for concurrent OBCPs
// The approach is derived from ESA's Micropython evolution for Leon processors,
// executed within the scope of ESA Contract No. 4000137198/22/NL/MGu/kk.
// A custom, non-OS dependent implementation of mp_state_ptr is provided
// instead of the one used in the original ESA contract.

void *mp_state_ptr(void);

#define MP_STATE_PTR ((mp_state_ctx_t*)mp_state_ptr())
#define MP_STATE_VM(x) (MP_STATE_PTR->vm.x)
#define MP_STATE_MEM(x) (MP_STATE_PTR->mem.x)
#define MP_STATE_THREAD(x) (MP_STATE_PTR->thread.x)
#define MP_STATE_MAIN_THREAD(x) (MP_STATE_PTR->thread.x)
#define MP_STATE_PORT MP_STATE_VM

#endif