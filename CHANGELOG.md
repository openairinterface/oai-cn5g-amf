# RELEASE NOTES: #

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

