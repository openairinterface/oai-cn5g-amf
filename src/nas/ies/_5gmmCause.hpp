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

#ifndef __5GMM_CAUSE_H
#define __5GMM_CAUSE_H

#include "Type3NasIe.hpp"

enum class _5gmmCauseEnum {
  kIllegalUE                                       = 0b00000011,
  kPeiNotAccepted                                  = 0b00000101,
  kIllegalMe                                       = 0b00000110,
  k5gsServicesNotAllowed                           = 0b00000111,
  kUeIdentityCannotBeDerivedByTheNetwork           = 0b00001001,
  kImplicitlyDe_registered                         = 0b00001010,
  kPlmnNotAllowed                                  = 0b00001011,
  kTrackingAreaNotAllowed                          = 0b00001100,
  kRoamingNotAllowedInThisTrackingArea             = 0b00001101,
  kNoSuitableCellsInTrackingArea                   = 0b00001111,
  kMacFailure                                      = 0b00010100,
  kSynchFailure                                    = 0b00010101,
  kCongestion                                      = 0b00010110,
  kUeSecurityCapabilitiesMismatch                  = 0b00010111,
  KSecurityModeRejectedUnspecified                 = 0b00011000,
  kNon_5GAuthenticationUnacceptable                = 0b00011010,
  kN1ModeNotAllowed                                = 0b00011011,
  kRestrictedServiceArea                           = 0b00011100,
  kRedirectionToEpcRequired                        = 0b00011111,
  kLandNotAvailable                                = 0b00101011,
  kMaximumNumberOfPduSessionsReached               = 0b01000001,
  kInsufficientResourcesForSpecificSliceAndDnn     = 0b01000011,
  kInsufficientResourcesForSpecificSlice           = 0b01000101,
  kNgKsiAlreadyInUse                               = 0b01000111,
  kNon_3gppAccessTo5gcnNotAllowed                  = 0b01001000,
  kServingNetworkNotAuthorized                     = 0b01001001,
  kTemporarilyNotAuthorizedForThisSnpn             = 0b01001010,
  kPermanentlyNotAuthorizedForThisSnpn             = 0b01001011,
  kPayloadWasNotForwarded                          = 0b01011010,
  kDnnNotSupportedOrNotSubscribedInTheSlice        = 0b01011011,
  kInsufficientUser_PlaneResourcesForThePduSession = 0b01011100,
  kSemanticallyIncorrectMessage                    = 0b01011111,
  kInvalidMandatoryInformation                     = 0b01100000,
  kMessageTypeNon_ExistentOrNotImplemented         = 0b01100001,
  kMessageTypeNotCompatibleWithTheProtocolState    = 0b01100010,
  kInformationElementNon_ExistentOrNotImplemented  = 0b01100011,
  kConditionalIEError                              = 0b01100100,
  kMessageNotCompatibleWithTheProtocolState        = 0b01100101,
  kProtocolError_Unspecified                       = 0b01101111
};

constexpr uint8_t k5gmmCauseMinimumLength = 1;
constexpr uint8_t k5gmmCauseMaximumLength = 2;

constexpr auto k5gmmCauseIeName = "5GMM Cause";

namespace nas {

class _5gmmCause : public Type3NasIe {
 public:
  _5gmmCause();
  _5gmmCause(uint8_t iei);
  _5gmmCause(uint8_t _iei, uint8_t value);
  ~_5gmmCause();

  static std::string GetIeName() { return k5gmmCauseIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void Set(uint8_t _iei, uint8_t value);
  void SetValue(uint8_t value);
  uint8_t GetValue() const;

 private:
  uint8_t value_;
};

}  // namespace nas

#endif
