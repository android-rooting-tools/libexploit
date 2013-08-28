/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

#ifndef MADV_MERGEABLE
#define MADV_MERGEABLE 12
#endif

extern void*  __mmap2(void*, size_t, int, int, int, size_t);

static void *free_address = (void *)0x10000000;

#define  MMAP2_SHIFT  12
void* mmap(void *addr, size_t size, int prot, int flags, int fd, long offset)
{
    void *ret;

    if (offset & ((1UL << MMAP2_SHIFT)-1)) {
        errno = EINVAL;
        return MAP_FAILED;
    }

    if (addr == NULL) {
      addr = free_address;
      free_address += size;
    }

    ret = __mmap2(addr, size, prot, flags, fd, (size_t)offset >> MMAP2_SHIFT);

    if (ret && (flags & (MAP_PRIVATE | MAP_ANONYMOUS)))
	    madvise(ret, size, MADV_MERGEABLE);

    return ret;
}
