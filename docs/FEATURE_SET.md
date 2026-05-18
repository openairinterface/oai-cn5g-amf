<!-- SPDX-License-Identifier: CC-BY-4.0 -->

<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OpenAirInterface AMF Feature Set</font></b>
    </td>
  </tr>
</table>

**Table of Contents**

1. [5GC Service Based Architecture](#1-5gc-service-based-architecture)
2. [OAI AMF Available Interfaces](#2-oai-smf-available-interfaces)
3. [OAI AMF Feature List](#3-oai-smf-feature-list)

# 1. 5GC Service Based Architecture #

![5GC SBA](./images/5gc_sba.png)

# 2. OAI AMF Available Interfaces #

| **ID** | **Interface** | **Status**         | **Comment**                                                               |
| ------ | ------------- | ------------------ | --------------------------------------------------------------------------|
| 1      | N1            | :heavy_check_mark: | Communicate with UE via NAS message                                       |
| 2      | N2            | :heavy_check_mark: | Communicate with gNB via NGAP message                                     |
| 3      | N8            | :heavy_check_mark: | Interface to/from UDM (e.g., retrieve UE subscription data)               |
| 4      | N11           | :heavy_check_mark: | Interface to/from SMF (e.g., N1N2MessageTransfer, PDU Session Services)   |
| 5      | N14           | :x:                | Interface between AMFs                                                    |
| 6      | N15           | :heavy_check_mark: | Interface between AMF and PCF                                             |

# 3. OAI AMF Feature List #

Based on document **3GPP TS 23.501 V16.14.0 §6.2.1**.

| **ID** | **Classification**                                                  | **Status**         | **Comments**                                |
| ------ | ------------------------------------------------------------------- | ------------------ | ------------------------------------------- |
| 1      | Termination of RAN CP interface (N2)                                | :heavy_check_mark: | Communicate with gNB via NGAP message       |
| 2      | Termination of NAS (N1)                                             | :heavy_check_mark: | Communicate with UE via NAS message         |
| 3      | NAS ciphering and integrity protection                              | :heavy_check_mark: |                                             |
| 4      | Registration management                                             | :heavy_check_mark: |                                             |
| 5      | Connection management                                               | :heavy_check_mark: |                                             |
| 6      | Reachability management                                             | :x:                |                                             |
| 7      | Mobility Management                                                 | :heavy_check_mark: | Support N2 Handover                         |
| 8      | Lawful intercept (for AMF events and interface to LI System)        | :x:                |                                             |
| 9      | Provide transport for SM messages between UE and SMF                | :heavy_check_mark: |                                             |
| 10     | Transparent proxy for routing SM messages                           | :x:                |                                             |
| 11     | Access Authentication                                               | :heavy_check_mark: |                                             |
| 12     | Access Authorization                                                | :heavy_check_mark: |                                             |
| 13     | Provide transport for SMS messages between UE and SMSF              | :x:                |                                             |
| 14     | Security Anchor Functionality (SEAF)                                | :heavy_check_mark: |                                             |
| 15     | Location Services management for regulatory services                | :x:                |                                             |
| 16     | Provide transport for Location Services messages between            |                    |                                             |
|        | UE and LMF as well as between RAN and LMF                           | :heavy_check_mark: |                                             |
| 17     | EPS Bearer ID allocation for interworking with EPS                  | :x:                |                                             |
| 18     | UE mobility event notification                                      | :heavy_check_mark: |                                             |
| 19     | Support for Control Plane CIoT 5GS Optimisation                     | :x:                |                                             |
| 20     | Provisioning of external parameters                                 | :x:                |                                             |
| 21     | Support non-3GPP access networks                                    | :x:                |                                             |

## Release 17.10 NAS Alignment

The following 3GPP TS 24.501 V17.10.1 / TS 23.502 V17.10.0 NAS changes have been aligned:

### Implemented (unconditional)
- **5GMM Capability**: Octets 4-7 decoded and stored; NSSRG, NSAG, UAS, and MPSIU bits exposed via named accessors (`SupportsNssrg()`, etc.)
- **IE Constants**: Message-scoped IEI aliases for NSSRG (0x70), NSAG (0x7C/0x73), Priority indicator (0xE-), Service-level-AA container (0x7B/0x72)
- **NAS codec safety**: Registration Request, Registration Accept, and Configuration Update Command optional IE loops use table-aware safe skip; Release 17 IEs (NSSRG, NSAG, Priority indicator) no longer truncate decode of earlier decoders
- **NSSRG Information IE** (TS 24.501 §9.11.3.82): TLV-E codec implemented; encode/decode round-trip tested
- **NSAG Information IE** (TS 24.501 §9.11.3.87): TLV-E codec implemented; IEI 0x7C in Registration Accept, 0x73 in Configuration Update Command. NSAG information IE encoded per TS 24.501 §9.11.3.87 over-the-air wire format (revised). Each NSAG entry encoded as: `[entry_length][nsag_id][snssai_list_length][{content_len,SST,[SD]}*][priority][optional TAI list]`.
- **Priority Indicator IE** (TS 24.501 §9.11.3.91): Type 1 TV half-octet codec implemented; encode/decode tested

### Feature-gated (requires config)
- **NSSRG emission** (`enable_nssrg: false` by default): NSSRG Information IE included in Registration Accept only when enabled, UE supports NSSRG, and subscription data has NSSRG lists. NSSF indicators (`ueSupNssrgInd`, `suppressNssrgInd`) sent only when enabled.
- **NSAG emission** (`enable_nsag: false` by default): NSAG Information IE included in Registration Accept and CUC when enabled and UE supports NSAG.
- **MPS Indicator Update** (`enable_mps_indicator_update: false` by default): MPS priority active state is populated from UDM `AccessAndMobilitySubscriptionData.mpsPriority` during registration. When access-identity-1 validity changes (via Nudm_SDM_Notification), `trigger_mps_indicator_update()` is called to send CUC with Priority Indicator IE (0xE-). Requires `enable_mps_indicator_update=true` and UE support of MPSIU. Known limitation: CUC not sent for non-MPSIU UEs (fallback per §4.5.2A is logged, re-registration required).
- **UAS/UUAA-MM** (`enable_uas_uuaa_mm: false` by default): UAS capability (`nas_ue_supports_uas`) is decoded and stored. `uas_authorized` context field is present (default false); populated via Nudm_SDM_Notification when `enable_uas_uuaa_mm=true`. Service-level-AA container (IEI 0x72) in Registration Request is safely skipped. Known Gap: Full `enable_uas_uuaa_mm=true` enforcement (PDU session rejection boundary, deregistration on subscription revocation, CUC Service-level-AA) is not yet implemented.
- **Nudm_SDM_Subscribe** (Stage 8): `Nudm_SDM_Subscribe` is issued after AM policy association during registration when `enable_access_and_mobility_subscription_data_retrieval=true`. The subscription monitors NSSAI and AM-data resources; UAS-data URI is added when `enable_uas_uuaa_mm=true`. On UE deregistration, the subscription is deleted (`Nudm_SDM_Unsubscribe` via DELETE).
- **Nudm_SDM_Notification handler** (Stage 8): Parses `ModificationNotification` and triggers UCU for NSSAI/NSAG subscription changes; calls `trigger_mps_indicator_update()` for AM-data changes; sets `pending_sdm_update` flag for idle UEs. Known Gap: AM-data notification MPS extraction — the handler detects AM-data changes but does not yet extract the specific new MPS priority value from `ModificationNotification.notifyItems` change content; the `trigger_mps_indicator_update()` call uses the current context value as a no-op placeholder until the change payload parsing is complete.
- **UCU/T3555** (Stage 4): Configuration Update infrastructure (T3555 retransmit, CUC Complete handler) implemented. CUC sending is not triggered from post-registration flow by default (UERANSIM compatibility).

### Unsupported / Out of scope
- ProSe relay, satellite NG-RAN, NSWO, non-3GPP registration, UE policy delivery (`enable_ue_policy_delivery: false`), local 5GSM decode (`enable_local_5gsm_decode: false`)
- Service-level-AA container in Configuration Update Command and Registration Accept: codec infrastructure exists (Stage 2 safe-skip parser); semantic handling deferred until `enable_uas_uuaa_mm=true` path is fully specified

### Known Gaps
- **T3550 retransmit stub**: The T3550 timer infrastructure is present and fires correctly; however, the Registration Accept NAS PDU retransmit action in [src/amf-app/amf_n1.cpp](src/amf-app/amf_n1.cpp) (inside `handle_t3550_expiry()`) is a `// TODO: re-send Registration Accept message` stub. Registration Accept is not actually re-sent on T3550 expiry. This is a pre-Release-17 gap and remains unresolved.
- **Full 5GSM codec**: AMF forwards N1 SM information to SMF as opaque bytes. Local 5GSM decode is not implemented.


