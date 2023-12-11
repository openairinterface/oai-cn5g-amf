/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#pragma once

constexpr uint8_t kIeIsOptional    = true;
constexpr uint8_t kIeIsNotOptional = false;

constexpr uint8_t kIei5gmmCause                        = 0x58;
constexpr uint8_t kIei5gsmCapability                   = 0x28;
constexpr uint8_t kIei5gsDrxParameters                 = 0x51;
constexpr uint8_t kIei5gsmCongestionReAttemptIndicator = 0x61;
constexpr uint8_t kIei5gsmNetworkFeatureSupport        = 0x17;
constexpr uint8_t kIei5gsMobileIdentityImeiSv          = 0x77;
constexpr uint8_t kIei5gsMobileIdentityNonImeiSvPei    = 0x78;
constexpr uint8_t kIei5gsRegistrationResult            = 0x44;
constexpr uint8_t kIei5gsTrackingAreaIdentity          = 0x52;
constexpr uint8_t kIei5gsTrackingAreaIdentityList      = 0x54;
constexpr uint8_t kIeiAdditionalInformation            = 0x24;
constexpr uint8_t kIeiAllowedSscMode                   = 0x0f;
constexpr uint8_t kIeiAlwaysOnPduSessionIndication     = 0x08;
constexpr uint8_t kIeiAlwaysOnPduSessionRequested      = 0x0b;
constexpr uint8_t kIeiAtsssContainer                   = 0x77;
constexpr uint8_t kIeiAuthorizedQosFlowDescriptions    = 0x79;
constexpr uint8_t kIeiAuthorizedQosRules               = 0x7a;
constexpr uint8_t kIeiBackOffTimerValue                = 0x37;
constexpr uint8_t kIeiControlPlaneOnlyIndication       = 0x0c;
constexpr uint8_t kIeiDnn                              = 0x25;
constexpr uint8_t kIeiDsTtEthernetPortMacAddress       = 0x6e;

constexpr uint8_t kIeiEthernetHeaderCompressionConfiguration = 0x1f;
constexpr uint8_t kIeiExtendedDrxParameters                  = 0x6e;
constexpr uint8_t kIeiExtendedProtocolConfigurationOptions =
    0x7b;  // To be verified
constexpr uint8_t kIeiIntegrityProtectionMaximumDataRate    = 0x13;
constexpr uint8_t kIeiIpHeaderCompressionConfiguration      = 0x66;
constexpr uint8_t kIeiMappedEpsBearerContexts               = 0x75;
constexpr uint8_t kIeiMaximumNumberOfSupportedPacketFilters = 0x55;
constexpr uint8_t kIeiNbN1ModeDrxParameters                 = 0x29;
constexpr uint8_t kIeiNSSAIAllowed                          = 0x15;
constexpr uint8_t kIeiNSSAIConfigured                       = 0x31;
constexpr uint8_t kIeiNSSAIPending                          = 0x39;
constexpr uint8_t kIeiNSSAIRequested                        = 0x2f;
constexpr uint8_t kIeiOldPduSessionId                       = 0x59;
constexpr uint8_t kIeiPduAddress                            = 0x29;
constexpr uint8_t kIeiPduSessionId                          = 0x12;
constexpr uint8_t kIeiPduSessionType                        = 0x09;
constexpr uint8_t kIeiPortManagementInformationContainer    = 0x74;
constexpr uint8_t kIeiReAttemptIndicator                    = 0x1d;
constexpr uint8_t kIeiRejectedNssaiRa                       = 0x11;
constexpr uint8_t kIeiRejectedNssaiDr                       = 0x6D;
constexpr uint8_t kIeiRejectedNssaiCuc                      = 0x11;

constexpr uint8_t kIeiRequestedQosFlowDescriptions = 0x79;
constexpr uint8_t kIeiRequestedQosRules            = 0x7a;
constexpr uint8_t kIeiRqTimerValue                 = 0x56;
constexpr uint8_t kIeiServingPlmnRateControlPsea   = 0x18;
constexpr uint8_t kIeiServingPlmnRateControlPsmc   = 0x1e;
constexpr uint8_t kIeiSessionAmbr                  = 0x2a;
constexpr uint8_t kIeiSmPduDnRequestContainer      = 0x39;
constexpr uint8_t kIeiSNssai                       = 0x22;
constexpr uint8_t kIeiSscMode                      = 0x0a;
constexpr uint8_t kIeiSuggestedInterfaceIdentifier = 0x29;
constexpr uint8_t kIeiUeDsTtResidenceTime          = 0x6f;
constexpr uint8_t kIeiFullNameForNetwork           = 0x43;
constexpr uint8_t kIeiShortNameForNetwork          = 0x45;

// OK
constexpr uint8_t kIeiPayloadContainerType     = 0x08;  // Should be verified
constexpr uint8_t kIeiRequestType              = 0x08;  // 8-(4 higher bits)
constexpr uint8_t kIeiNetworkSlicingIndication = 0x09;  // 9-(4 higher bits)

constexpr uint8_t kIeiNssaiInclusionMode        = 0x0A;  // A-(4 higher bits)
constexpr uint8_t kIeiMaPduSessionInformation   = 0x0A;  // A-(4 higher bits)
constexpr uint8_t kIeiMicoIndication            = 0x0B;  // B-(4 higher bits)
constexpr uint8_t kIeiNasKeySetIdentifier       = 0x0C;  // C-(4 higher bits)
constexpr uint8_t kIeiNon3gppNwProvidedPolicies = 0x0D;  // D-(4 higher bits)
constexpr uint8_t kIeiConfigurationUpdateIndication =
    0x0D;                                                  // D-(4 higher bits)
constexpr uint8_t kIeiImeisvRequest               = 0x0E;  // E-(4 higher bits)
constexpr uint8_t kIeiReleaseAssistanceIndication = 0x0F;  // F-(4 higher bits)

constexpr uint8_t kIei5gmmCapability              = 0x10;
constexpr uint8_t kIeiUeUsageSetting              = 0x18;
constexpr uint8_t kIeiS1UeSecurityCapability      = 0x19;
constexpr uint8_t kIeiAuthenticationParameterAutn = 0x20;
constexpr uint8_t kIeiAuthenticationParameterRand = 0x21;
constexpr uint8_t kIei5gsNetworkFeatureSupport    = 0x21;

constexpr uint8_t kIeiAllowedPduSessionStatus         = 0x25;
constexpr uint8_t kIeiPduSessionReactivationResult    = 0x26;
constexpr uint8_t kIeiServiceAreaList                 = 0x27;
constexpr uint8_t kIeiUeStatus                        = 0x2b;
constexpr uint8_t kIeiAuthenticationResponseParameter = 0x2D;

constexpr uint8_t kIeiUeSecurityCapability = 0x2e;
constexpr uint8_t kIeiUeNetworkCapability  = 0x17;

constexpr uint8_t kIeiAuthenticationFailureParameter = 0x30;

constexpr uint8_t kIeiAdditional5gSecurityInformation = 0x36;
constexpr uint8_t kIeiAbba                            = 0x38;

constexpr uint8_t kIeiUplinkDataStatus = 0x40;

constexpr uint8_t kT3502Value = 0x16;

constexpr uint8_t kEquivalentPlmns = 0x4A;

constexpr uint8_t kIeiPduSessionStatus         = 0x50;
constexpr uint8_t kIei5gsUpdateType            = 0x53;
constexpr uint8_t kIeiEpsNasSecurityAlgorithms = 0x57;
constexpr uint8_t kT3346Value                  = 0x5f;

constexpr uint8_t kIeiEpsBearerContextStatus = 0x60;
constexpr uint8_t kIeiUeRadioCapabilityId    = 0x67;
constexpr uint8_t kIeiRejectedNssaiRr        = 0x69;
constexpr uint8_t kIeiGprsTimer3T3324        = 0x6A;
constexpr uint8_t kIeiGprsTimer3T3448        = 0x6B;
constexpr uint8_t kIeiGprsTimer3T3447        = 0x6C;
constexpr uint8_t kIeiGprsTimer3T3512        = 0x5E;
constexpr uint8_t kIeiGprsTimer3BackOffTimer = 0x37;

constexpr uint8_t kIeiGprsTimer2Non3gppDeregistration = 0x5D;
constexpr uint8_t kIeiGprsTimer2T3502                 = 0x16;
constexpr uint8_t kIeiGprsTimer2T3346                 = 0x5F;

constexpr uint8_t kIeiEpsNasMessageContainer                 = 0x70;
constexpr uint8_t kIeiNasMessageContainer                    = 0x71;
constexpr uint8_t kIeiPduSessionReactivationResultErrorCause = 0x72;
constexpr uint8_t kIeiSorTransparentContainer                = 0x73;
constexpr uint8_t kIeiLadnIndication                         = 0x74;
constexpr uint8_t kIeiLadnInformation                        = 0x79;
constexpr uint8_t kIei5gGuti                                 = 0x77;

constexpr uint8_t kIeiEapMessage       = 0x78;
constexpr uint8_t kIeiPayloadContainer = 0x7b;
