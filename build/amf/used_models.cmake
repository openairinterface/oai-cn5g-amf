################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################

## This file is used to specify the common models and utils this library is using
## DO NOT JUST COPY THIS FILE FROM OTHER NFs. The reasoning behind this is to only compile used files to optimize
## build speed
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/common_model/common_model.cmake)

# Add common model dependencies from AMF API model
list(APPEND USED_COMMON_MODEL_SRC_FILES
        ${COMMON_MODEL_DIR}/AccessTokenErr.cpp
        ${COMMON_MODEL_DIR}/AccessTokenReq.cpp
        ${COMMON_MODEL_DIR}/AccessType.cpp
        ${COMMON_MODEL_DIR}/Ambr.cpp
        ${COMMON_MODEL_DIR}/Area.cpp
        ${COMMON_MODEL_DIR}/Arp.cpp
        ${COMMON_MODEL_DIR}/CellGlobalId.cpp
        ${COMMON_MODEL_DIR}/CoreNetworkType.cpp
        ${COMMON_MODEL_DIR}/CoreNetworkType_anyOf.cpp
        ${COMMON_MODEL_DIR}/Ecgi.cpp
        ${COMMON_MODEL_DIR}/EutraLocation.cpp
        ${COMMON_MODEL_DIR}/GeraLocation.cpp
        ${COMMON_MODEL_DIR}/GNbId.cpp
        ${COMMON_MODEL_DIR}/GlobalRanNodeId.cpp
        ${COMMON_MODEL_DIR}/Guami.cpp
        ${COMMON_MODEL_DIR}/Helpers.cpp
        ${COMMON_MODEL_DIR}/HfcNodeId.cpp
        ${COMMON_MODEL_DIR}/InvalidParam.cpp
        ${COMMON_MODEL_DIR}/Ipv6Addr.cpp
        ${COMMON_MODEL_DIR}/LineType.cpp
        ${COMMON_MODEL_DIR}/LineType_anyOf.cpp
        ${COMMON_MODEL_DIR}/LinksValueSchema.cpp
        ${COMMON_MODEL_DIR}/LocationAreaId.cpp
        ${COMMON_MODEL_DIR}/N3gaLocation.cpp
        ${COMMON_MODEL_DIR}/NFType.cpp
        ${COMMON_MODEL_DIR}/NFType_anyOf.cpp
        ${COMMON_MODEL_DIR}/Ncgi.cpp
        ${COMMON_MODEL_DIR}/NgApCause.cpp
        ${COMMON_MODEL_DIR}/NrLocation.cpp
        ${COMMON_MODEL_DIR}/PatchItem.cpp
        ${COMMON_MODEL_DIR}/PatchOperation.cpp
        ${COMMON_MODEL_DIR}/PatchOperation_anyOf.cpp
        ${COMMON_MODEL_DIR}/PlmnId.cpp
        ${COMMON_MODEL_DIR}/PlmnIdNid.cpp
        ${COMMON_MODEL_DIR}/PreemptionCapability.cpp
        ${COMMON_MODEL_DIR}/PreemptionCapability_anyOf.cpp
        ${COMMON_MODEL_DIR}/PreemptionVulnerability.cpp
        ${COMMON_MODEL_DIR}/PreemptionVulnerability_anyOf.cpp
        ${COMMON_MODEL_DIR}/PresenceInfo.cpp
        ${COMMON_MODEL_DIR}/PresenceState.cpp
        ${COMMON_MODEL_DIR}/PresenceState_anyOf.cpp
        ${COMMON_MODEL_DIR}/ProblemDetails.cpp
        ${COMMON_MODEL_DIR}/RatType.cpp
        ${COMMON_MODEL_DIR}/RatType_anyOf.cpp
        ${COMMON_MODEL_DIR}/RefToBinaryData.cpp
        ${COMMON_MODEL_DIR}/RestrictionType.cpp
        ${COMMON_MODEL_DIR}/RestrictionType_anyOf.cpp
        ${COMMON_MODEL_DIR}/RoutingAreaId.cpp
        ${COMMON_MODEL_DIR}/ServiceAreaId.cpp
        ${COMMON_MODEL_DIR}/ServiceAreaRestriction.cpp
        ${COMMON_MODEL_DIR}/Snssai.cpp
        ${COMMON_MODEL_DIR}/Tai.cpp
        ${COMMON_MODEL_DIR}/TnapId.cpp
        ${COMMON_MODEL_DIR}/TraceData.cpp
        ${COMMON_MODEL_DIR}/TraceDepth.cpp
        ${COMMON_MODEL_DIR}/TraceDepth_anyOf.cpp
        ${COMMON_MODEL_DIR}/TransportProtocol.cpp
        ${COMMON_MODEL_DIR}/TransportProtocol_anyOf.cpp
        ${COMMON_MODEL_DIR}/TwapId.cpp
        ${COMMON_MODEL_DIR}/UserLocation.cpp
        ${COMMON_MODEL_DIR}/UtraLocation.cpp
)

# finally, we have to include common_model.cmake (has to be last
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/common_model/common_model.cmake)
