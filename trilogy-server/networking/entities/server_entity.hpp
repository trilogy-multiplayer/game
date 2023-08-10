#pragma once

#include <cstdint>
#include <definitions.hpp>

#include <networking/packets/packet_player_sync_data.hpp>
#include <string>
#include <vendor/librg/librg.h>

class c_server_entity
{
public:
	c_server_entity(int32_t network_id, std::string name, librg_peer* peer);

	e_entity_types entity_type;
	int32_t network_id;
	librg_peer* client_peer;

	std::string name;
	packet_player_sync_data* player_sync_data;
};