#pragma once

#ifndef TRILOGY_MEMORY_H
#define TRILOGY_MEMORY_H

#include "common.hpp"

#include "module.hpp"
#include "patterns.hpp"

#include "scripting/scripting.hpp"

#include <sdk/sdk.hpp>
#include <networking/packets/packet_player_sync_data.hpp>

class c_memory : public c_singleton<c_memory> {
private:
	using sdk_find_player_ped_t = sdk_ped*(*)(int32_t player_id);
	using sdk_calc_screen_coords_t = bool(*)(const sdk_vec3_t& in, sdk_vec3_t* out, float* outw, float* outh, bool farclip, bool unk);
public:

	class sdk_camera
	{
	public:
		char pad_0x0000[0x198]; //0x0000
		float camera_heading; //0x0198 
	}; //Size=0x019C

	class sdk_camera_data_front
	{
	public:
		sdk_vec3_t m_front_pos;
		sdk_vec3_t m_pos;
		sdk_vec3_t m_unk_pos;
		sdk_vec3_t m_unk_offset_pos;
		char pad_0x0030[0x10]; //0x0030
	}; //Size=0x0040

	/**
	  * This dont belong here, need to move
	  */
#define SDK_LOCAL_PLAYER 0
#define SDK_REMOTE_PLAYER 2
#define SDK_CONTEXT_PLAYER 3

	sdk_find_player_ped_t o_sdk_find_player_ped;
	sdk_find_player_ped_t sdk_find_player_ped;

	int8_t* sdk_player_in_focus;

    float* time_step;
	sdk_ped** sdk_world_players;

    int32_t* sdk_ped_pool_padding;
	int64_t* sdk_ped_pool;

    hid::hid_mapping* sdk_hid_mapping;
	sdk_camera* sdk_current_camera;
	sdk_camera_data_front* sdk_current_camera_data_front;

	sdk_calc_screen_coords_t sdk_calc_screen_coords;

	void initialize();
};

#endif
