// SPDX-License-Identifier: AGPL-3.0-only
/*
 * This file is licensed under AGPL-3.0-only.
 * MODULE_LICENSE("GPL") is required by the Linux kernel
 * infrastructure and does not override the AGPL-3.0 license.
 */
/*
 * TID — The Instant Destroyer
 * Linux kernel module for compiler-resistant secure memory
 * and cache zeroization on x86_64.
 *
 * Author : Ahmad Qasim Mohammad Hassan
 * ORCID  : 0009-0001-4360-0802
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/sched/mm.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <asm/cpufeature.h>

#include "tid_uapi.h"

#define TID_DEVICE_NAME  "tid"
#define TID_MAX_SIZE     (16UL * 1024UL * 1024UL)
#define TID_CACHE_LINE   64UL

/* ----------------------------------------------------------------
 * Core routine: three sequential phases
 *   1) Compiler-resistant wipe using REP STOSQ
 *   2) Cache flush CLFLUSHOPT over the entire original range
 *   3) Full memory barrier MFENCE
 * ---------------------------------------------------------------- */
static noinline void tid_zeroize_core(void *buf, size_t size)
{
    void      *flush_base = buf;
    void      *addr       = buf;
    size_t     qwords     = size >> 3;
    size_t     remainder  = size & 0x7UL;
    uintptr_t  start, end, line;

    if (qwords) {
        asm volatile ("rep stosq"
                      : "+D"(addr), "+c"(qwords)
                      : "a"(0ULL)
                      : "memory");
    }
    if (remainder) {
        u8 *p = (u8 *)addr;
        while (remainder--)
            *p++ = 0;
    }

    start = (uintptr_t)flush_base & ~(TID_CACHE_LINE - 1UL);
    end   = ((uintptr_t)flush_base + size + TID_CACHE_LINE - 1UL)
            & ~(TID_CACHE_LINE - 1UL);

    for (line = start; line < end; line += TID_CACHE_LINE)
        asm volatile ("clflushopt (%0)"
                      :: "r"((void *)line)
                      : "memory");

    asm volatile ("mfence" ::: "memory");
}

/* ----------------------------------------------------------------
 * Self-test on load: verifies functional correctness
 * ---------------------------------------------------------------- */
static int __init tid_self_test(void)
{
    static const size_t sizes[] = {
        1, 7, 8, 9, 15, 16, 17, 31, 32, 33,
        37, 63, 64, 65, 100, 127, 128, 256, 511,
        512, 1024, 4095, 4096, 65535, 65536
    };
    size_t i, j;
    u8 *buf;

    for (i = 0; i < ARRAY_SIZE(sizes); i++) {
        buf = kmalloc(sizes[i], GFP_KERNEL);
        if (!buf)
            return -ENOMEM;
        memset(buf, 0xAB, sizes[i]);
        tid_zeroize_core(buf, sizes[i]);
        for (j = 0; j < sizes[i]; j++) {
            if (buf[j] != 0) {
                pr_err("TID: self-test FAILED size=%zu byte[%zu]=0x%02x\n",
                       sizes[i], j, buf[j]);
                kfree(buf);
                return -EINVAL;
            }
        }
        kfree(buf);
    }
    pr_info("TID: self-test PASSED (%zu sizes)\n", ARRAY_SIZE(sizes));
    return 0;
}

/* ----------------------------------------------------------------
 * ioctl handler: pins user pages and executes the wipe
 * ---------------------------------------------------------------- */
static long tid_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    struct tid_args  uargs;
    struct page    **pages = NULL;
    void            *kbuf  = NULL;
    unsigned long    npages, off, first_page;
    long             pinned = 0;
    ktime_t          t0, t1;
    long             ret = 0;

    if (cmd != TID_IOCTL_ZERO)
        return -ENOTTY;
    if (copy_from_user(&uargs, (void __user *)arg, sizeof(uargs)))
        return -EFAULT;
    if (uargs.size == 0 || uargs.size > TID_MAX_SIZE)
        return -EINVAL;

    first_page = uargs.user_addr & PAGE_MASK;
    off        = uargs.user_addr & (PAGE_SIZE - 1);
    npages     = (off + uargs.size + PAGE_SIZE - 1) >> PAGE_SHIFT;

    pages = kmalloc_array(npages, sizeof(*pages), GFP_KERNEL);
    if (!pages)
        return -ENOMEM;

    pinned = pin_user_pages_fast(first_page, npages,
                                 FOLL_WRITE | FOLL_LONGTERM, pages);
    if (pinned != (long)npages) {
        ret = (pinned < 0) ? pinned : -EFAULT;
        goto out_unpin;
    }

    kbuf = vmap(pages, npages, VM_MAP, PAGE_KERNEL);
    if (!kbuf) {
        ret = -ENOMEM;
        goto out_unpin;
    }

    t0 = ktime_get();
    tid_zeroize_core((u8 *)kbuf + off, uargs.size);
    t1 = ktime_get();

    vunmap(kbuf);

    uargs.elapsed_ns   = ktime_to_ns(ktime_sub(t1, t0));
    uargs.success_code = TID_SUCCESS_CODE;

    if (copy_to_user((void __user *)arg, &uargs, sizeof(uargs)))
        ret = -EFAULT;

out_unpin:
    if (pinned > 0)
        unpin_user_pages(pages, pinned);
    kfree(pages);
    return ret;
}

/* ----------------------------------------------------------------
 * Device registration
 * ---------------------------------------------------------------- */
static const struct file_operations tid_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = tid_ioctl,
    .compat_ioctl   = tid_ioctl,
};

static struct miscdevice tid_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = TID_DEVICE_NAME,
    .fops  = &tid_fops,
    .mode  = 0600,
};

static int __init tid_init(void)
{
    int rc;

    if (!boot_cpu_has(X86_FEATURE_CLFLUSHOPT)) {
        pr_err("TID: CPU lacks CLFLUSHOPT support\n");
        return -ENODEV;
    }

    rc = tid_self_test();
    if (rc)
        return rc;

    rc = misc_register(&tid_misc);
    if (rc) {
        pr_err("TID: misc_register failed (%d)\n", rc);
        return rc;
    }
    pr_info("TID: loaded (/dev/%s)\n", TID_DEVICE_NAME);
    return 0;
}

static void __exit tid_exit(void)
{
    misc_deregister(&tid_misc);
    pr_info("TID: unloaded\n");
}

module_init(tid_init);
module_exit(tid_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ahmad Qasim Mohammad Hassan <ORCID: 0009-0001-4360-0802>");
MODULE_DESCRIPTION("The Instant Destroyer: secure memory + cache zeroization");
MODULE_VERSION("1.0");
