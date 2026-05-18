# TID Diagrams — Complete Visual Reference

> Author: Ahmad Qasim Mohammad Hassan | ORCID: [0009-0001-4360-0802](https://orcid.org/0009-0001-4360-0802)
> Version: v2.0 | Year: 2026

---

This folder contains the complete visual reference for the TID project — 10 diagrams covering system architecture, attack mechanics, experimental results, and security comparisons.

---

## Diagrams Overview

| # | Title | Description |
|---|-------|-------------|
| 1 | **TID System Architecture** | Operation flow from the application layer down to RAM — showing how ioctl(), LFENCE, REP STOSQ, CLFLUSHOPT, and MFENCE work together |
| 2 | **PROTECT+ZERO Protocol** | Sensitive data lifecycle across three phases: PROTECT before use, data in use (372ns window), and ZERO after use |
| 3 | **Attack Timeline** | TID execution vs. attacker probe timing — shows the attacker gets only ~3.3 measurements inside the 372ns window and finds nothing |
| 4 | **Cache Comparison: Before and After TID** | Side-by-side cache state across three scenarios: No TID / TID v1.0 / TID v2.0 |
| 5 | **Performance Comparison** | CPU cycles comparison — v1.0 (78 cycles, Cache HIT, leak) vs. v2.0 (286 cycles, Cache MISS, attack blind) — ratio: 3.7× |
| 6 | **TID vs Existing Libraries** | Feature comparison table: memset / explicit_bzero / OpenSSL / libsodium / TID v2.0 / Intel SGX |
| 7 | **Statistical Key Reconstruction Attack** | Shows how an attacker reconstructs an AES key byte-by-byte via Flush+Reload on TID v1 (987/1000 hits) — and why TID v2 breaks the chain (0/1000) |
| 8 | **Security Solution Ladder** | Ranks solutions from weakest to strongest — TID v2.0 is the maximum achievable at software level without dedicated hardware |
| 9 | **Physical Minimum Latency of TID** | Per-instruction timing breakdown — the 300–372ns total is a CPU physics constraint, not a code limitation |
| 10 | **Experimental Proof Summary** | All three tests on AMD EPYC 9B14 — Basic Attack (1,000 samples), Timing Window (10,000 samples), CPU Shield (5,000 samples) |

---

## Key Numbers

| Metric | Value |
|--------|-------|
| TID destruction window | 372 ns |
| v1.0 attacker latency | 78 cycles (Cache HIT — leak) |
| v2.0 attacker latency | 286 cycles (Cache MISS — safe) |
| Protection ratio | 3.7× |
| Attacker probes inside window | ~3.3 only |
| Self-test sizes | 25 (1 byte → 65,536 bytes) |
| Test hardware | AMD EPYC 9B14 — Linux 6.14.11 |

---

## Files

| File | Contents |
|------|----------|
| `TID_Diagrams.pptx` | Original editable PowerPoint — 12 slides |
| `TID_Diagrams.pdf` | PDF export — available in `docs/` |

---

## License

© 2026 Ahmad Qasim Mohammad Hassan — AGPL-3.0
