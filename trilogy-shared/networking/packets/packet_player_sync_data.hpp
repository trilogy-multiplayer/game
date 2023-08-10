#pragma once
#include <cstring> 
#include <cstdint>
#include <vendor/librg/zpl/zpl_math.h>

enum class e_hid_mapping_current_state : int16_t {
	JUST_RELEASED,
	NOT_PRESSED,
	PRESSED = 3
};

class hid_mapping_state
{
public:
	e_hid_mapping_current_state m_cur_state; //0x0000 
	__int8 N0000010D; //0x0002 
	__int8 N00000110; //0x0003 
}; //Size=0x0004

class hid_mapping
{
public:
	hid_mapping_state m_states[102]; //0x0000 
	char pad_0x0198[0x674]; //0x0198
}; //Size=0x080C

class packet_player_sync_data {
public:
	/*float current_rotation;
	float current_heading;

	zplm_vec3_t move_speed;

	CCompressedControls controls;

	uint8_t weapon_id;

	uint8_t health;
	uint8_t armour;*/

	hid_mapping mapping;
};