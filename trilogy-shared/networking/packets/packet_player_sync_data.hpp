#pragma once

#include <cstring> 
#include <cstdint>
#include <vendor/librg/zpl/zpl_math.h>

//TODO: move into shared
#include "../../../trilogy-client/sdk/sdk_vector.hpp"

#include <string>

/**
  * This thing needs asap a rework lol
  */

namespace hid {
	enum class e_hid_mapping_current_state : int16_t {
		JUST_RELEASED,
		NOT_PRESSED,
		PRESSED = 3
	};

	enum e_hid_mapping : int16_t {
		WALK_FORWARD = 29,
		WALK_LEFT = 42,
		WALK_RIGHT = 44,
		WALK_BACKWARD = 43,

		JUMP = 68,
		SPRINT = 54,
		CROUCH = 57,
		SNEAK = 66,

		MOUSE_ATTACK = 0,
		MOUSE_AIM = 1,
		MOUSE_MIDDLE = 2,
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
		hid_mapping_state m_keyboard_states[103]; //0x0000 
		char pad_0x019C[0x148]; //0x019C
		__int16 N000000F8; //0x02E4 
		__int16 N00000100; //0x02E6 
		hid_mapping_state m_mouse_states[3]; //0x02E8 
	}; //Size=0x02F4


	class hid_compressed_mapping {
	public:
		hid_mapping_state key_walk_forward;
		hid_mapping_state key_walk_left;
		hid_mapping_state key_walk_right;
		hid_mapping_state key_walk_backward;

		hid_mapping_state key_jump;
		hid_mapping_state key_sprint;
		hid_mapping_state key_crouch;
		hid_mapping_state key_sneak;

		hid_mapping_state mouse_attack;
		hid_mapping_state mouse_aim;

		bool is_empty() {
			return key_walk_forward.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_walk_left.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_walk_right.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_walk_backward.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&

				key_jump.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_sprint.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_crouch.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				key_sneak.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&

				mouse_attack.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED &&
				mouse_aim.m_cur_state == e_hid_mapping_current_state::NOT_PRESSED;
		}
	};

	inline hid_compressed_mapping compress_mapping(hid_mapping input_mapping) {
		hid_compressed_mapping compressed_mapping{};

		compressed_mapping.key_walk_forward = input_mapping.m_keyboard_states[e_hid_mapping::WALK_FORWARD];
		compressed_mapping.key_walk_left = input_mapping.m_keyboard_states[e_hid_mapping::WALK_LEFT];
		compressed_mapping.key_walk_right = input_mapping.m_keyboard_states[e_hid_mapping::WALK_RIGHT];
		compressed_mapping.key_walk_backward = input_mapping.m_keyboard_states[e_hid_mapping::WALK_BACKWARD];

		compressed_mapping.key_jump = input_mapping.m_keyboard_states[e_hid_mapping::JUMP];
		compressed_mapping.key_sprint = input_mapping.m_keyboard_states[e_hid_mapping::SPRINT];
		compressed_mapping.key_crouch = input_mapping.m_keyboard_states[e_hid_mapping::CROUCH];
		compressed_mapping.key_sneak = input_mapping.m_keyboard_states[e_hid_mapping::SNEAK];

		compressed_mapping.mouse_attack = input_mapping.m_mouse_states[e_hid_mapping::MOUSE_ATTACK];
		compressed_mapping.mouse_aim = input_mapping.m_mouse_states[e_hid_mapping::MOUSE_AIM];

		return compressed_mapping;
	}

	inline hid_mapping decompress_mapping(hid_compressed_mapping compressed_mapping, hid_mapping input_mapping) {
		input_mapping.m_keyboard_states[e_hid_mapping::WALK_FORWARD] = compressed_mapping.key_walk_forward;
		input_mapping.m_keyboard_states[e_hid_mapping::WALK_LEFT] = compressed_mapping.key_walk_left;
		input_mapping.m_keyboard_states[e_hid_mapping::WALK_RIGHT] = compressed_mapping.key_walk_right;
		input_mapping.m_keyboard_states[e_hid_mapping::WALK_BACKWARD] = compressed_mapping.key_walk_backward;

		input_mapping.m_keyboard_states[e_hid_mapping::JUMP] = compressed_mapping.key_jump;
		input_mapping.m_keyboard_states[e_hid_mapping::SPRINT] = compressed_mapping.key_sprint;
		input_mapping.m_keyboard_states[e_hid_mapping::CROUCH] = compressed_mapping.key_crouch;
		input_mapping.m_keyboard_states[e_hid_mapping::SNEAK] = compressed_mapping.key_sneak;

		input_mapping.m_mouse_states[e_hid_mapping::MOUSE_ATTACK] = compressed_mapping.mouse_attack;
		input_mapping.m_mouse_states[e_hid_mapping::MOUSE_AIM] = compressed_mapping.mouse_aim;

		return input_mapping;
	}
}

class packet_player_sync_data {
public:
	/*float current_rotation;
	float current_heading;

	zplm_vec3_t move_speed;

	CCompressedControls controls;

	uint8_t weapon_id;

	uint8_t health;
	uint8_t armour;*/
	sdk_vec3_t camera_front;

	// int16_t ped_state;
	float current_rotation_a;
	float current_rotation_b;

	float current_move_x;
	float current_move_y;

	sdk_vec3_t move_speed;
	// hid_compressed_mapping mapping;
};