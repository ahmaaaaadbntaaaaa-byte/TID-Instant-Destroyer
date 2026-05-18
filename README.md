# TID — The Instant Destroyer

**Independent security research in the field of protection against cache-based side-channel attacks.**

## Overview
TID is a specialized protocol designed to eliminate sensitive data remnants (such as cryptographic keys) from CPU cache layers (L1, L2, and L3) immediately after use. It addresses a critical security gap where data persists in hardware structures even after being wiped from system memory (RAM).

## Key Achievements
- **Verified Defense:** Effectively mitigates **Flush+Reload** and similar microarchitectural side-channel attacks.
- **Measurable Performance:** - **Unprotected Access:** ~78 CPU cycles (Cache Hit).
  - **TID Protected Access:** ~286 CPU cycles (Cache Miss).
  - **Security Margin:** Provides a **3.7x latency barrier**, successfully blinding potential attackers.
- **Hardware-Level Precision:** Utilizes optimized CPU instructions (`CLFLUSHOPT`, `MFENCE`) to ensure physical cache eviction.

## Author & Identity
- **Lead Researcher:** Ahmad Qasim Mohammad Hassan
- **ORCID:** [0009-0001-4360-0802](https://orcid.org/0009-0001-4360-0802)
- **Professional Profile:** [LinkedIn Profile](https://www.linkedin.com/in/ahmad-hasan-5aa031267/)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.17585929.svg)](https://doi.org/10.5281/zenodo.17585929)


## Documentation & License
- Detailed research papers and technical execution reports are located in the `/research` folder.
- This project is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**.
