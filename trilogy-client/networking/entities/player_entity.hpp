#pragma once

#include <cstdint>
#include <networking/entities/network_entity.hpp>
#include <definitions.hpp>
#include <memory/memory.hpp>
#include <sdk/sdk_ped.hpp>

#include <networking/packets/packet_player_sync_data.hpp>

class c_player_entity : public c_network_entity
{
public:
	c_player_entity(int32_t network_id, std::string name, bool is_local, sdk_vec3_t pos);

	/**
	  * Network entity inherits here
	  */
	// void on_client_stream(librg_event* event) override;

	int char_id = 0; // 0x32
	int player_id = 0; // 0x32

	bool is_local;

	std::string name = "";
	sdk_vec3_t position = sdk_vec3_t(0,0,0);

	packet_player_sync_data* player_sync_data;
};