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

#include "_5gsMobileIdentity.hpp"
#include "MicoIndication.hpp"
#include "NetworkSlicingIndication.hpp"
#include "PduSessionStatus.hpp"
#include "UEStatus.hpp"
#include "NasMmPlainHeader.hpp"
#include "_5gmmCapability.hpp"
#include "_5gsRegistrationType.hpp"
#include "Abba.hpp"
#include "Additional5gSecurityInformation.hpp"
#include "AdditionalInformation.hpp"
#include "AllowedPDUSessionStatus.hpp"
#include "AuthenticationFailureParameter.hpp"
#include "AuthenticationParameterAutn.hpp"
#include "AuthenticationParameterRand.hpp"
#include "AuthenticationResponseParameter.hpp"
#include "Dnn.hpp"
#include "EapMessage.hpp"
#include "EpsBearerContextStatus.hpp"
#include "EpsNasMessageContainer.hpp"
#include "EpsNasSecurityAlgorithms.hpp"
#include "ExtendedDrxParameters.hpp"
#include "GprsTimer2.hpp"
#include "GprsTimer3.hpp"
#include "ImeisvRequest.hpp"
#include "LadnIndication.hpp"
#include "LadnInformation.hpp"
#include "MaPduSessionInformation.hpp"
#include "NasMessageContainer.hpp"
#include "NasSecurityAlgorithms.hpp"
#include "Nssai.hpp"
#include "NssaiInclusionMode.hpp"
#include "NasKeySetIdentifier.hpp"
#include "Non3gppNwProvidedPolicies.hpp"
#include "PduSessionIdentity2.hpp"
#include "PduSessionReactivationResult.hpp"
#include "PduSessionReactivationResultErrorCause.hpp"
#include "PlmnList.hpp"
#include "PayloadContainer.hpp"
#include "PayloadContainerType.hpp"
#include "RejectedNssai.hpp"
#include "ReleaseAssistanceIndication.hpp"
#include "RequestType.hpp"
#include "S1UeSecurityCapability.hpp"
#include "SorTransparentContainer.hpp"
#include "SNssai.hpp"
#include "ServiceType.hpp"
#include "UENetworkCapability.hpp"
#include "UEUsageSetting.hpp"
#include "UESecurityCapability.hpp"
#include "UeRadioCapabilityId.hpp"
#include "UplinkDataStatus.hpp"
#include "_5gmmCause.hpp"
#include "_5gsDeregistrationType.hpp"
#include "_5gsTrackingAreaIdList.hpp"
#include "_5gsDrxParameters.hpp"
#include "_5gsIdentityType.hpp"
#include "_5gsNetworkFeatureSupport.hpp"
#include "_5gsRegistrationResult.hpp"
#include "_5gsTrackingAreaIdentity.hpp"
#include "_5gsUpdateType.hpp"
#include "struct.hpp"
#include "IeConst.hpp"
#include "NetworkName.hpp"
