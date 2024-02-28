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

#ifndef _3GPP_TS_24501_H_
#define _3GPP_TS_24501_H_

#include <string>

#include "3gpp_24.007.hpp"

// Security Header Type
constexpr uint8_t kPlain5gsMessage                                     = 0b0000;
constexpr uint8_t kIntegrityProtected                                  = 0b0001;
constexpr uint8_t kIntegrityProtectedAndCiphered                       = 0b0010;
constexpr uint8_t kIntegrityProtectedWithNewSecurityContext            = 0b0011;
constexpr uint8_t kIntegrityProtectedAndCipheredWithNewSecurityContext = 0b0100;

// Message Types for 5GS Mobility Management
constexpr uint8_t k5gsMobilityManagementMessageTypeUnknown = 0b10000000;

constexpr uint8_t kRegistrationRequest                = 0b01000001;
constexpr uint8_t kRegistrationAccept                 = 0b01000010;
constexpr uint8_t kRegistrationComplete               = 0b01000011;
constexpr uint8_t kRegistrationReject                 = 0b01000100;
constexpr uint8_t kDeregistrationRequestUeOriginating = 0b01000101;
constexpr uint8_t kDeregistrationAcceptUeOriginating  = 0b01000110;
constexpr uint8_t kDeregistrationRequestUeTerminated  = 0b01000111;
constexpr uint8_t kDeregistrationAcceptUeTerminated   = 0b01001000;

constexpr uint8_t kServiceRequest             = 0b01001100;
constexpr uint8_t kServiceReject              = 0b01001101;
constexpr uint8_t kServiceAccept              = 0b01001110;
constexpr uint8_t kControlPlaneServiceRequest = 0b01001111;

constexpr uint8_t kNetworkSliceSpecificAuthenticationCommand  = 0b01010000;
constexpr uint8_t kNetworkSliceSpecificAuthenticationComplete = 0b01010001;
constexpr uint8_t kNetworkSliceSpecificAuthenticationResult   = 0b01010010;
constexpr uint8_t kConfigurationUpdateCommand                 = 0b01010100;
constexpr uint8_t kConfigurationUpdateComplete                = 0b01010101;
constexpr uint8_t kAuthenticationRequest                      = 0b01010110;
constexpr uint8_t kAuthenticationResponse                     = 0b01010111;
constexpr uint8_t kAuthenticationReject                       = 0b01011000;
constexpr uint8_t kAuthenticationFailure                      = 0b01011001;
constexpr uint8_t kAuthenticationResult                       = 0b01011010;
constexpr uint8_t kIdentityRequest                            = 0b01011011;
constexpr uint8_t kIdentityResponse                           = 0b01011100;
constexpr uint8_t kSecurityModeCommand                        = 0b01011101;
constexpr uint8_t kSecurityModeComplete                       = 0b01011110;
constexpr uint8_t kSecurityModeReject                         = 0b01011111;

constexpr uint8_t k5gmmStatus                      = 0b01100100;
constexpr uint8_t kMessageTypeNotification         = 0b01100101;
constexpr uint8_t kMessageTypeNotificationResponse = 0b01100110;
constexpr uint8_t kUlNasTransport                  = 0b01100111;
constexpr uint8_t kDlNasTransport                  = 0b01101000;

// Message Types for 5GS Session Management
constexpr uint8_t k5gsSessionManagementMessageTypeUnknown = 0b11000000;

constexpr uint8_t kPduSessionEstablishmentRequest   = 0b11000001;
constexpr uint8_t kPduSessionEstablishmentAccept    = 0b11000010;
constexpr uint8_t kPduSessionEstablishmentReject    = 0b11000011;
constexpr uint8_t kPduSessionAuthenticationCommand  = 0b11000101;
constexpr uint8_t kPduSessionAuthenticationComplete = 0b11000110;
constexpr uint8_t kPduSessionAuthenticationResult   = 0b11000111;

constexpr uint8_t kPduSessionModificationRequest       = 0b11001001;
constexpr uint8_t kPduSessionModificationReject        = 0b11001010;
constexpr uint8_t kPduSessionModificationCommand       = 0b11001011;
constexpr uint8_t kPduSessionModificationComplete      = 0b11001100;
constexpr uint8_t kPduSessionModificationCommandReject = 0b11001101;

constexpr uint8_t kPduSessionReleaseRequest  = 0b11010001;
constexpr uint8_t kPduSessionReleaseReject   = 0b11010010;
constexpr uint8_t kPduSessionReleaseCommand  = 0b11010011;
constexpr uint8_t kPduSessionReleaseComplete = 0b11010100;

constexpr uint8_t k5gsmStatus = 0b11010110;

// Registration Type
constexpr bool kNoFollowOnReqPending = false;
constexpr bool kFollowOnReqPending   = true;

constexpr uint8_t kInitialRegistration          = 0b001;
constexpr uint8_t kMobilityRegistrationUpdating = 0b010;
constexpr uint8_t kPeriodicRegistrationUpdating = 0b011;
constexpr uint8_t kEmergencyRegistration        = 0b100;

// NAS Key Set Identifier
constexpr uint8_t kNasKeySetIdentifierNative       = 0b0;
constexpr uint8_t kNasKeySetIdentifierMapped       = 0b1;
constexpr uint8_t kNasKeySetIdentifierNotAvailable = 0b111;

// 5GS Mobile Identity
constexpr uint8_t kNoIdentity                = 0b000;
constexpr uint8_t kSuci                      = 0b001;
constexpr uint8_t k5gGuti                    = 0b010;
constexpr uint8_t kImei                      = 0b011;
constexpr uint8_t k5gSTmsi                   = 0b100;
constexpr uint8_t kImeisv                    = 0b101;
constexpr uint8_t kMacAddress                = 0b110;
constexpr uint8_t k5gsMobileIdentityMaxValue = kMacAddress;

constexpr uint8_t kEvenIdentity = 0;
constexpr uint8_t kOddIdentity  = 1;

// SUPI format
constexpr uint8_t kSupiFormatImsi                      = 0b000;
constexpr uint8_t kSupiFormatNetworkSpecificIdentifier = 0b001;

// Scheme
constexpr uint8_t kNullScheme          = 0b0000;
constexpr uint8_t kEciesSchemeProfileA = 0b0001;
constexpr uint8_t kEciesSchemeProfileB = 0b0010;

constexpr uint8_t kHomeNetworkPki0WhenPsi0 = 0b00000000;
constexpr uint8_t kHomeNetworkPkiReserved  = 0b11111111;

// Security algorithms
constexpr uint8_t kIa0_5g     = 0b000;
constexpr uint8_t kIa1_128_5g = 0b001;
constexpr uint8_t kIa2_128_5g = 0b010;

constexpr uint8_t kEa0_5g     = 0b000;
constexpr uint8_t kEa1_128_5g = 0b001;
constexpr uint8_t kEa2_128_5g = 0b010;

// 5G MM CAUSE value for 5g mobility management (Annex A)
constexpr uint8_t k5gmmCauseIllegalUe                 = 3;
constexpr uint8_t k5gmmCauseSynchFailure              = 0b00010101;  // 21
constexpr uint8_t k5gmmCauseImplicitlyDeRegistered    = 10;
constexpr uint8_t k5gmmCauseUeIdentityCannotBeDerived = 9;
constexpr uint8_t k5gmmCauseNgksiAlreadyInUse         = 0b01000111;  // 71

// A.5 Causes related to invalid messages
//------------------------------------------------------------------------------
constexpr uint8_t k5gmmCauseSemanticallyIncorrect     = 95;
constexpr uint8_t k5gmmCauseInvalidMandatoryInfo      = 96;
constexpr uint8_t k5gmmCauseMessageTypeNotImplemented = 97;
constexpr uint8_t k5gmmCauseMessageTypeNotCompatible  = 98;
constexpr uint8_t k5gmmCauseIeNotImplemented          = 99;
constexpr uint8_t k5gmmCauseConditionalIeError        = 100;
constexpr uint8_t k5gmmCauseMessageNotCompatible      = 101;
constexpr uint8_t k5gmmCauseProtocolError             = 111;

// UL NAS TRANSPORT payload container type
constexpr uint8_t kN1SmInformation         = 0x01;
constexpr uint8_t kSmsContainer            = 0x02;
constexpr uint8_t kLtePositioningProtocol  = 0x03;
constexpr uint8_t kSorTransparentContainer = 0x04;
constexpr uint8_t kUePolicyContainer       = 0x05;
constexpr uint8_t kUeParametersUpdate      = 0x06;
constexpr uint8_t kMultiplePayloads        = 0x0f;

constexpr uint8_t kPduSessionInitialRequest          = 0b001;
constexpr uint8_t kExistingPduSession                = 0b010;
constexpr uint8_t kPduSessionInitialEmergencyRequest = 0b011;
constexpr uint8_t kExistingEmergencyPduSession       = 0b100;
constexpr uint8_t kPduSessionTypeModificationRequest = 0b101;
constexpr uint8_t kMaPduRequest                      = 0b110;

constexpr uint8_t kDeregistrationTypeMask = 0b00001000;

constexpr uint8_t kNasMessageMinLength = 3;

constexpr int KEncodeDecodeError = -1;
constexpr int KEncodeDecodeOK    = 0;

constexpr int kT3502TimerDefaultValueMin = 12;  // 12 minutes

// Table 10.3.1 @3GPP TS 24.501 V16.1.0 (2019-06)
constexpr int kT3512TimerValueSec = 3240;  // 54 minutes
constexpr int kT3512TimerValueMin = 54;    // 54 minutes
constexpr int kMobileReachableTimerNoEmergencyServicesMin =
    (kT3512TimerValueMin + 4);  // T3512 + 4, not for emergency services
constexpr int kImplicitDeregistrationTimerMin = (kT3512TimerValueMin + 4);

enum class _5g_ia_e {
  _5G_IA0 = 0,
  _5G_IA1 = 1,
  _5G_IA2 = 2,
  _5G_IA3 = 3,
  _5G_IA4 = 4,
  _5G_IA5 = 5,
  _5G_IA6 = 6,
  _5G_IA7 = 7
};

static std::string get_5g_ia_str(_5g_ia_e e) {
  switch (e) {
    case _5g_ia_e::_5G_IA0: {
      return "5G_IA0";
    } break;
    case _5g_ia_e::_5G_IA1: {
      return "5G_IA1";
    } break;
    case _5g_ia_e::_5G_IA2: {
      return "5G_IA2";
    } break;
    case _5g_ia_e::_5G_IA3: {
      return "5G_IA3";
    } break;
    case _5g_ia_e::_5G_IA4: {
      return "5G_IA4";
    } break;
    case _5g_ia_e::_5G_IA5: {
      return "5G_IA5";
    } break;
    case _5g_ia_e::_5G_IA6: {
      return "5G_IA6";
    } break;
    case _5g_ia_e::_5G_IA7: {
      return "5G_IA7";
    } break;
    default: {
      return "UNKNOWN 5GS ENCRYPTION ALGORITHM";
    }
  }
}

static _5g_ia_e get_5g_ia(std::string ia) {
  if (!ia.compare("NIA0")) {
    return _5g_ia_e::_5G_IA0;
  }
  if (!ia.compare("NIA1")) {
    return _5g_ia_e::_5G_IA1;
  }
  if (!ia.compare("NIA2")) {
    return _5g_ia_e::_5G_IA2;
  }
  if (!ia.compare("NIA3")) {
    return _5g_ia_e::_5G_IA3;
  }
  if (!ia.compare("NIA4")) {
    return _5g_ia_e::_5G_IA4;
  }
  if (!ia.compare("NIA5")) {
    return _5g_ia_e::_5G_IA5;
  }
  if (!ia.compare("NIA6")) {
    return _5g_ia_e::_5G_IA6;
  }
  if (!ia.compare("NIA7")) {
    return _5g_ia_e::_5G_IA7;
  }
  return _5g_ia_e::_5G_IA0;
}

enum class _5g_ea_e {
  _5G_EA0 = 0,
  _5G_EA1 = 1,
  _5G_EA2 = 2,
  _5G_EA3 = 3,
  _5G_EA4 = 4,
  _5G_EA5 = 5,
  _5G_EA6 = 6,
  _5G_EA7 = 7
};

static std::string get_5g_ea_str(_5g_ea_e e) {
  switch (e) {
    case _5g_ea_e::_5G_EA0: {
      return "5G_EA0";
    } break;
    case _5g_ea_e::_5G_EA1: {
      return "5G_EA1";
    } break;
    case _5g_ea_e::_5G_EA2: {
      return "5G_EA2";
    } break;
    case _5g_ea_e::_5G_EA3: {
      return "5G_EA3";
    } break;
    case _5g_ea_e::_5G_EA4: {
      return "5G_EA4";
    } break;
    case _5g_ea_e::_5G_EA5: {
      return "5G_EA5";
    } break;
    case _5g_ea_e::_5G_EA6: {
      return "5G_EA6";
    } break;
    case _5g_ea_e::_5G_EA7: {
      return "5G_EA7";
    } break;
    default: {
      return "UNKNOWN 5GS INTEGRITY ALGORITHM";
    }
  }
}

static _5g_ea_e get_5g_ea(std::string ea) {
  if (!ea.compare("NEA0")) {
    return _5g_ea_e::_5G_EA0;
  }
  if (!ea.compare("NEA1")) {
    return _5g_ea_e::_5G_EA1;
  }
  if (!ea.compare("NEA2")) {
    return _5g_ea_e::_5G_EA2;
  }
  if (!ea.compare("NEA3")) {
    return _5g_ea_e::_5G_EA3;
  }
  if (!ea.compare("NEA4")) {
    return _5g_ea_e::_5G_EA4;
  }
  if (!ea.compare("NEA5")) {
    return _5g_ea_e::_5G_EA5;
  }
  if (!ea.compare("NEA6")) {
    return _5g_ea_e::_5G_EA6;
  }
  if (!ea.compare("NEA7")) {
    return _5g_ea_e::_5G_EA7;
  }
  return _5g_ea_e::_5G_EA0;
}
#endif
