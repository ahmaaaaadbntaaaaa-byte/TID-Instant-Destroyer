// SPDX-License-Identifier: AGPL-3.0
/*
 * TID — The Instant Destroyer
 * User-space command-line tool to invoke TID via /dev/tid
 *
 * Author : Ahmad Qasim Mohammad Hassan
 * ORCID  : 0009-0001-4360-0802
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "tid_uapi.h"

int main(int argc, char **argv)
{
    size_t          sz;
    void           *buf;
    int             fd;
    struct tid_args a = {0};

    if (argc != 2) {
        fprintf(stderr, "usage: %s <size_bytes>\n", argv[0]);
        return 2;
    }
    sz = strtoul(argv[1], NULL, 0);

    buf = aligned_alloc(64, (sz + 63) & ~63UL);
    if (!buf) { perror("alloc"); return 1; }
    memset(buf, 0xAB, sz);

    fd = open("/dev/tid", O_RDWR);
    if (fd < 0) { perror("open /dev/tid"); free(buf); return 1; }

    a.size      = sz;
    a.user_addr = (unsigned long)buf;

    if (ioctl(fd, TID_IOCTL_ZERO, &a) < 0) {
        perror("ioctl");
        close(fd); free(buf); return 1;
    }
    close(fd);

    if (a.success_code != TID_SUCCESS_CODE) {
        fprintf(stderr, "bad success_code: 0x%08x\n", a.success_code);
        free(buf); return 1;
    }

    /* byte-by-byte verification */
    for (size_t i = 0; i < sz; i++) {
        if (((unsigned char *)buf)[i] != 0) {
            fprintf(stderr, "byte %zu = 0x%02x (NOT zero)\n",
                    i, ((unsigned char *)buf)[i]);
            free(buf); return 1;
        }
    }

    printf("OK size=%zu elapsed=%llu ns\n",
           sz, (unsigned long long)a.elapsed_ns);
    free(buf);
    return 0;
}
