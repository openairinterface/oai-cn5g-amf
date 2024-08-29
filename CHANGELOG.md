# RELEASE NOTES: #

## v2.1.0 -- August 2024 ##

* Features
  - Adding NRPPA protocol support
  - Support Non UE N2 Info message
  - Support Multiple PDU sessions service request
  - Adding gNB statistics
  - Adding NGAP Utils
  - Adding LTTNg tracepoints for logging
  - Using hexadecimal values for SD in all APIs
  - Updating UE info statistics on AMF after service request
  - Exposing NCGI in UE Location Report notification
  - Use HTTP Request Timeout from conf file
* Fixes
  - Fix issue for JSON iterator comparison
  - Fix multiple sessions response handling
  - Fix NF registration/deregistration when discovering NRFs from NSSF
  - Fix PDU session resource context request null pointer
  - Fix issue for retrieving GUTI from 5G-S-TMSI
  - Fix HTTP/2 server shutdown
  - Fix shutdown threads procedure
  - Fix UE Context Release procedure
  - Fix buffer-overflow issue
  - Fix AMF UE NGAP ID (uint64_t, 40 bits)
  - Fix Nas buffer allocation length
  - Fix issue for N2 handover
  - Fix GUTI decode
  - Fix NAS Decode: check buffer length before decoding
  - Fix issue for mobility registration update procedure
  - Fix HTTP client by using SBI task to send HTTP request
* Tech Debt
  - Cleanup common utils
  - Stopping support for RHEL8/Rocky8 in favor of RHEL9/Rocky9
  - Move NAS to common src git submodule
  - Move NGAP to common src git submodule
  - Move AMF SBI models to common src git submodule
  - Move conversions from AMF to common src git submodule
  - Separate all in one implementation
  - HTTP client cpr library refactoring effort

## v2.0.1 -- January 2024 ##

* Features
  - NGAP AMF status indication
  - Add UE Retention Information support
  - Update NAS with Rel 16.14.0 IEs
    *  Refactoring code for Encode/Decode functions
    *  Cleanup NAS lib
* Fixes
  - Fix typo for N1N2MessageSubscribe
  - Fix issue when receiving PDU session reject from SMF
* Tech debt
  - Reformating of the SCTP code
  - Refactor promise handling
  - Removing dependencies to libconfig++ (Only YAML file can be read as configuration)

## v2.0.0 -- December 2023 ##

* Features
  - Support YAML configuration file
    * Yaml validation default value
  - Service request handling
  - Update API Server name/structure
  - N2 info notify
  - Updated NGAP library to 38.413 V16.14.0
  - Location Report Notification for N2 Handover 
  - LMF Determine Location & DownlinkUeAssociatedNrppaTranport support
* Fixes
  - Fixed: Add NSSAI info in AMF profile to send to NRF
  - Fixed: Http1/2 - only spawning one HTTP server
  - Fixed: HTTP2 support for Configuration/Location-related APIs
  - Fixed: Display NRCellID in hex to associate between UE and gNB
  - Fixed: Use AMF ID as string to register to NRF
  - Added missing HTTP2 APIs for N1 Message Notify, N1N2MessageSubscribe/Unsubscribe, Status Notification
  - Free allocated memory in Destructor of NGAP msg instead of using Delete operator
  - Added retry mechanism for NF Registration/Update
  - Removed unnecessary config parameter: generate_random (database)
  - Fixed N1/N2 message unsubscribe API
  - Fixed issue when decoding PduSessionResourceSetupResponse
* Tech debt
  - Updated common models to 3GPP TS 29.571 Release 16.13.0 and moved them to the shared common submodule
  - Switching to clang-format-12
  - Published image on Docker-Hub is using now Ubuntu-22 as base image
    * Ubuntu-18 is no longer supported
* Miscellaneous
  - Code refactoring: ITTI between SBI and other tasks, NGAP IEs
  - Code cleanup: NAS IEs, N2, removed usused code, Used new instead of malloc/calloc for N2

## v1.5.1 -- May 2023 ##

* Fix Service Request handling (Service Type: Signalling)
* Fix N2 handover
* Support usage of SUPI-type IMSI
* Fix mapping between RAN UE NGAP ID and UE context
* Fix SBI port for UDM in case of FQDN resolution
* Fix TMSI handling
* Fix PDU Session Ressource Release Response message
* Fix issue for decoding MSIN with an odd number of digits
* Fix usage of lower-case for DNN value (case insensitive)
* Fix: NSSF port number was not read from config file
* Code Refactoring for:
  * NAS layer
  * Registration Complete Message
  * UE context refactor
  * Logging mechanism (runtime log level selection)
  * Installation / build scripts
  * Continuous Integration scripts
* Published image on Docker-Hub is using now Ubuntu-20 as base image
  * We will soon obsolete the build system for Ubuntu18.04

## v1.5.0 -- January 2023 ##

* Minimal fix for Decoding RegistrationRequest from COTS UE
* Fix small issue for NGAP lib
* NGAP IES code refactoring
* Fix PDU session release
* Refactor NGAP message callback
* Fix nake ptr
* feat(fqdn): giving some time for FQDN resolution
* Fix UL NAS transport
* Fix typo for Rejected NSSAI
* PDU session release supported
* Update SST type (int instead of string) in conf file
* Events exposure: location report with empty supi fixed
* small fix for event exposure loss of connectivity
* Minor fix for encoding no SD in PduSessionResourceSetupRequest
* Fix SST/SD on cots ue
* Docker images improvements
* Fixed docker exit by catching SIGTERM
* release mode does not use libasan anymore --> allocation of 20T virtual memory is no longer done
* Ubuntu22 and cgroup2 support

## v1.4.0 -- July 2022 ##

* Fix SCTP NG Shutdown issue
* Fix 100% CPU consumption issue
* Fix RAN UE ID display
* Fix issue for TMSI and GUTI value
* Fix for handling S-NSSAI 
* Remove OPC value from AMF configuration
* Support for Ubuntu 20.04 bare-metal installation
* Official images produced by CI are pushed to `oaisoftwarealliance` Docker-Hub Team account
* Reduce image size


## v1.3.0 -- January 2022 ##

* Periodic Registration Update
* Support for Event Exposure (Registration State Report)
* Implicit Deregistration Timer
* Mobile Reachable Timer
* Mobility Registration Update
* NRF Selection (via NSSF)
* Fix for validation of SMF Info
* Fix RAN UE NGAP Id
* Fix SMF Selection SD
* Fix NSSAI mismatch
* Fix Service Request
* Fix HTTP2
* Fix MCC such as 001
* Docker optimization

## v1.2.1 -- October 2021 ##

* Fix build issue
* Tentative fix for ngKSI already in use
* Initial implementation for Mobility registration update

## v1.2.0 -- September 2021 ##

* AUSF connection
* Support PDU Session Resource Modify procedure
* Support HTTP2
* Support AMF Event Exposure Services
* Fix NIA0, NEA2
* Fix potential AMF crash with UE/NGAP/NAS context
* Fix N2 Handover
* Fix Paging procedures

## v1.1.0 -- July 2021 ##

* Session Release
* NRF registration
  - with FQDN DNS resolution
* Multiple PDU support
* Bug fixes
* Full support for Ubuntu18 and RHEL8

## v1.0.0 -- September 2020 ##

* Initial release

