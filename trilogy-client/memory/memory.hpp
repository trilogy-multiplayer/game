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
	using sdk_find_player_ped_t = int64_t(*)(int32_t player_id);
public:
	sdk_find_player_ped_t o_sdk_find_player_ped;
	sdk_find_player_ped_t sdk_find_player_ped;
	int8_t* sdk_player_in_focus;

    float* time_step;

    int32_t* sdk_ped_pool_padding;
	int64_t* sdk_ped_pool;

    hid_mapping* sdk_hid_mapping;

	void initialize();
};

#endif
