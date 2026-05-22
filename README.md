TID — The Instant Destroyer
Independent security research in the field of protection against cache-based side-channel attacks.
�
�
Overview
TID is a specialized protocol designed to eliminate sensitive data remnants (such as cryptographic keys) from CPU cache layers (L1, L2, \text{ and } L3) immediately after use. It addresses a critical security gap where data persists in hardware structures even after being wiped from system memory (RAM).
The Problem No Library Solves
Every security library — OpenSSL, libsodium, glibc — wipes RAM after use. But none of them flush the CPU cache.
After sodium_memzero() or OPENSSL_cleanse() runs:
RAM → zeroed ✓
L1/L2/L3 Cache → still contains your key ✗
A Flush+Reload attacker measures cache access time with RDTSC and reconstructs the key byte by byte — even after your "secure wipe".
Key Achievements
Verified Defense: Effectively mitigates Flush+Reload and similar microarchitectural side-channel attacks.
Measurable Performance:
Unprotected Access: ~78 CPU cycles (Cache Hit)
TID Protected Access: ~286 CPU cycles (Cache Miss)
Security Margin: Provides a 3.7x latency barrier, successfully blinding potential attackers.
Hardware-Level Precision: Utilizes optimized CPU instructions (CLFLUSHOPT, MFENCE) to ensure physical cache eviction.
Comparison With Existing Libraries
Solution
Wipes RAM
Flushes Cache
Verdict
memset()
✗ compiler removes it
✗
Unreliable
explicit_bzero()
✓
✗
Partial
OPENSSL_cleanse()
✓
✗
Partial
sodium_memzero()
✓
✗
Partial
TID v2.0
✓
✓
Best software
Intel SGX
✓
Automatic
Requires dedicated hardware
Author & Identity
Lead Researcher: Ahmad Qasim Mohammad Hassan
ORCID: 0009-0001-4360-0802
Professional Profile: LinkedIn Profile
Academic Attestation: 
Documentation & License
Detailed research papers and technical execution reports are located in the /research folder.
This project is licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).
Support the Research
TID is an independent security research project. Bare-metal hardware testing on high-end enterprise processors is resource-intensive and costly. By sponsoring this project, you directly support the infrastructure costs required for ongoing validation and security research.
