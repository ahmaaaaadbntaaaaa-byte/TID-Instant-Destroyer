#ifndef _TID_UAPI_H
#define _TID_UAPI_H

#include <linux/types.h>
#include <linux/ioctl.h>

/*
 * TID — The Instant Destroyer
 * Shared ABI between kernel module and user space.
 * Author: Ahmad Qasim Mohammad Hassan
 * ORCID:  0009-0001-4360-0802
 *
 * Data structure exchanged between user space and the kernel.
 * ABI alignment fixed at 32 bytes for strict binary compatibility.
 */
struct tid_args {
    __u64 size;          /* size of region to wipe (bytes)         */
    __u64 user_addr;     /* user-space address of the region       */
    __u64 elapsed_ns;    /* execution time in nanoseconds (output) */
    __u32 success_code;  /* 0x5449444F on success (TIDO)          */
    __u32 _pad;          /* ABI padding                            */
};

#define TID_MAGIC        'T'
#define TID_IOCTL_ZERO   _IOWR(TID_MAGIC, 1, struct tid_args)

#define TID_SUCCESS_CODE 0x5449444Fu

#endif /* _TID_UAPI_H */
