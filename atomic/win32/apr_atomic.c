/* Copyright 2000-2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "apr.h"
#include "apr_atomic.h"
#include "apr_thread_mutex.h"

APR_DECLARE(apr_status_t) apr_atomic_init(apr_pool_t *p)
{
    return APR_SUCCESS;
}

APR_DECLARE(void *) apr_atomic_casptr(volatile void **mem, void *with, const void *cmp)
{
    return InterlockedCompareExchangePointer(mem, with, cmp);
}

/* 
 * Remapping function pointer type to accept apr_uint32_t's type-safely
 * as the arguments for as our apr_atomic_foo32 Functions
 */
typedef WINBASEAPI apr_uint32_t (WINAPI * apr_atomic_win32_ptr_fn)
    (apr_uint32_t volatile *);
typedef WINBASEAPI apr_uint32_t (WINAPI * apr_atomic_win32_ptr_val_fn)
    (apr_uint32_t volatile *, 
     apr_uint32_t);
typedef WINBASEAPI apr_uint32_t (WINAPI * apr_atomic_win32_ptr_val_val_fn)
    (apr_uint32_t volatile *, 
     apr_uint32_t, apr_uint32_t);

APR_DECLARE(apr_uint32_t) apr_atomic_add32(volatile apr_uint32_t *mem, apr_uint32_t val)
{
    return ((apr_atomic_win32_ptr_val_fn)InterlockedExchangeAdd)(mem, val);
}

APR_DECLARE(void) apr_atomic_sub32(volatile apr_uint32_t *mem, apr_uint32_t val)
{
    ((apr_atomic_win32_ptr_val_fn)InterlockedExchangeAdd)(mem, -val);
}

APR_DECLARE(apr_uint32_t) apr_atomic_inc32(volatile apr_uint32_t *mem)
{
    /* we return old value, win32 returns new value :( */
    return ((apr_atomic_win32_ptr_fn)InterlockedIncrement)(mem) - 1;
}

APR_DECLARE(int) apr_atomic_dec32(volatile apr_uint32_t *mem)
{
    return ((apr_atomic_win32_ptr_fn)InterlockedDecrement)(mem);
}

APR_DECLARE(void) apr_atomic_set32(volatile apr_uint32_t *mem, apr_uint32_t val)
{
    ((apr_atomic_win32_ptr_val_fn)InterlockedExchange)(mem, val);
}

APR_DECLARE(apr_uint32_t) apr_atomic_read32(volatile apr_uint32_t *mem)
{
    return *mem;
}

APR_DECLARE(apr_uint32_t) apr_atomic_cas32(volatile apr_uint32_t *mem, apr_uint32_t with,
                                           apr_uint32_t cmp)
{
    return ((apr_atomic_win32_ptr_val_val_fn)InterlockedCompareExchange)(mem, with, cmp);
}

APR_DECLARE(apr_uint32_t) apr_atomic_xchg32(volatile apr_uint32_t *mem, apr_uint32_t val)
{
    return ((apr_atomic_win32_ptr_val_fn)InterlockedExchange)(mem, val);
}
