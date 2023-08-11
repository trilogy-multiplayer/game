#pragma once

#include "sdk_vector.hpp"
#include "sdk_script_commands.hpp"
#include "sdk_running_script.hpp"

#include "sdk/sdk_ped.hpp"
#include "sdk/sdk_pool.hpp"

/**
  * TODO:
  * Split these into header files
  */

//enum class e_hid_mapping_current_state : int16_t {
//	JUST_RELEASED,
//	NOT_PRESSED,
//	PRESSED = 3
//};
//
//class hid_mapping_state
//{
//public:
//	e_hid_mapping_current_state m_cur_state; //0x0000 
//	__int8 N0000010D; //0x0002 
//	__int8 N00000110; //0x0003 
//}; //Size=0x0004
//
//class hid_mapping
//{
//public:
//	hid_mapping_state m_states[102]; //0x0000 
//	char pad_0x0198[0x674]; //0x0198
//}; //Size=0x080C