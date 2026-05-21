# TID — The Instant Destroyer

**Independent security research in the field of protection against cache-based side-channel attacks.**

---

## Overview
TID is a specialized protocol designed to eliminate sensitive data remnants (such as cryptographic keys) from CPU cache layers ($L1, L2, \text{ and } L3$) immediately after use. It addresses a critical security gap where data persists in hardware structures even after being wiped from system memory (RAM).

## Key Achievements
* **Verified Defense:** Effectively mitigates **Flush+Reload** and similar microarchitectural side-channel attacks.
* **Measurable Performance:** * **Unprotected Access:** ~78 CPU cycles (Cache Hit).
  * **TID Protected Access:** ~286 CPU cycles (Cache Miss).
  * **Security Margin:** Provides a **3.7x latency barrier**, successfully blinding potential attackers.
* **Hardware-Level Precision:** Utilizes optimized CPU instructions (`CLFLUSHOPT`, `MFENCE`) to ensure physical cache eviction.

---

## Author & Identity

* **Lead Researcher:** Ahmad Qasim Mohammad Hassan
* **ORCID:** <a href="https://orcid.org/0009-0001-4360-0802" target="_blank">0009-0001-4360-0802</a>
* **Professional Profile:** <a href="https://www.linkedin.com/in/ahmad-hasan-5aa031267/" target="_blank">LinkedIn Profile</a>
* **Academic Attestation:** <a href="https://doi.org/10.5281/zenodo.17585929"><img src="https://zenodo.org/badge/DOI/10.5281/zenodo.17585929.svg" alt="DOI" style="vertical-align:middle;"></a>

---

## Documentation & License
* Detailed research papers and technical execution reports are located in the `/research` folder.
* This project is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**.
---

## ☕ Support This Research
If you find TID (The Instant Destroyer) useful for your security research or projects, please consider supporting my work. Contributions help cover the costs of the dedicated infrastructure used for research and testing.

### 🔬 Transparency Note: Testing Environment
To ensure rigorous validation, TID v2.0 is tested on **dedicated bare-metal server infrastructure** (AMD EPYC). While this is a remote server environment rather than a local workstation, we utilize bare-metal instances to ensure full CPU access without the interference of hypervisor virtualization. This allows for precise, reproducible measurements of hardware cache-line operations (CLFLUSHOPT) that are critical for this research.

**Support my independent research via:**
[**Donate via PayPal**](https://paypal.me/ahmadbnt)

---
