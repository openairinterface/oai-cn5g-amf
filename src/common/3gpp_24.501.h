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

#ifndef FILE_3GPP_24_501_SEEN
#define FILE_3GPP_24_501_SEEN

#define T3502_TIMER_DEFAULT_VALUE_MIN 12  // 12 minutes

// Table 10.3.1 @3GPP TS 24.501 V16.1.0 (2019-06)
#define T3512_TIMER_VALUE_SEC 3240  // 54 minutes
#define T3512_TIMER_VALUE_MIN 54    // 54 minutes
#define MOBILE_REACHABLE_TIMER_NO_EMERGENCY_SERVICES_MIN                       \
  (T3512_TIMER_VALUE_MIN + 4)  // T3512 + 4, not for emergency services
#define IMPLICIT_DEREGISTRATION_TIMER_MIN (T3512_TIMER_VALUE_MIN + 4)

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
