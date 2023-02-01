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

#include "5GSMobileIdentity.hpp"
#include "MICOIndication.hpp"
#include "NetworkSlicingIndication.hpp"
#include "PDUSessionStatus.hpp"
#include "UEStatus.hpp"
#include "NasMmPlainHeader.hpp"
#include "5GMMCapability.hpp"
#include "5GSRegistrationType.hpp"
#include "ABBA.hpp"
#include "Additional5gSecurityInformation.hpp"
#include "AdditionalInformation.hpp"
#include "AllowedPDUSessionStatus.hpp"
#include "AuthenticationFailureParameter.hpp"
#include "Authentication_Parameter_AUTN.hpp"
#include "Authentication_Parameter_RAND.hpp"
#include "AuthenticationResponseParameter.hpp"
#include "DNN.hpp"
#include "EapMessage.hpp"
#include "EpsBearerContextStatus.hpp"
#include "EPS_NAS_Message_Container.hpp"
#include "EpsNasSecurityAlgorithms.hpp"
#include "Extended_DRX_Parameters.hpp"
#include "GprsTimer2.hpp"
#include "GprsTimer3.hpp"
#include "ImeisvRequest.hpp"
#include "LADN_Indication.hpp"
#include "MaPduSessionInformation.hpp"
#include "NasMessageContainer.hpp"
#include "NasSecurityAlgorithms.hpp"
#include "NSSAI.hpp"
#include "NssaiInclusionMode.hpp"
#include "NasKeySetIdentifier.hpp"
#include "Non_3GPP_NW_Provided_Policies.hpp"
#include "PduSessionIdentity2.hpp"
#include "PDU_Session_Reactivation_Result.hpp"
#include "PDU_Session_Reactivation_Result_Error_Cause.hpp"
#include "PLMN_List.hpp"
#include "Payload_Container.hpp"
#include "PayloadContainerType.hpp"
#include "Rejected_NSSAI.hpp"
#include "ReleaseAssistanceIndication.hpp"
#include "RequestType.hpp"
#include "S1UeSecurityCapability.hpp"
#include "SorTransparentContainer.hpp"
#include "S_NSSAI.hpp"
#include "ServiceType.hpp"
#include "UENetworkCapability.hpp"
#include "UEUsageSetting.hpp"
#include "UESecurityCapability.hpp"
#include "UeRadioCapabilityId.hpp"
#include "UplinkDataStatus.hpp"
#include "_5gmmCause.hpp"
#include "_5GSDeregistrationType.hpp"
#include "_5GSTrackingAreaIdList.hpp"
#include "_5GS_DRX_Parameters.hpp"
#include "_5gsIdentityType.hpp"
#include "_5GS_Network_Feature_Support.hpp"
#include "_5GS_Registration_Result.hpp"
#include "_5GSTrackingAreaIdentity.hpp"
#include "_5gsUpdateType.hpp"
#include "struct.hpp"
#include "Ie_Const.hpp"
#include "NetworkName.hpp"
