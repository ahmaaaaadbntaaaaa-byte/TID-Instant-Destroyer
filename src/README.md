# TID — Source Code

This directory contains the complete source code of TID (The Instant Destroyer).

**Author:** Ahmad Qasim Mohammad Hassan  
**ORCID:** 0009-0001-4360-0802

## Files

| File | Description |
|------|-------------|
| `tid_uapi.h` | Shared ABI between kernel module and user space |
| `tid.c` | Linux kernel module — Ring 0 — CLFLUSHOPT + REP STOSQ + MFENCE |
| `tid_user.c` | User-space tool to invoke TID via /dev/tid |
| `Makefile` | Build and load the kernel module |

## Build & Run

```bash
# Build
make

# Verify CLFLUSHOPT in binary
objdump -d tid.ko | grep clflushopt

# Load module
sudo insmod tid.ko

# Test (256 bytes)
gcc -O2 -o tid_user tid_user.c
./tid_user 256

# Unload
sudo rmmod tidREADME.md
