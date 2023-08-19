#pragma once

#include <cstdint>
#include <networking/entities/network_entity.hpp>
#include <definitions.hpp>

#include <networking/packets/packet_player_sync_data.hpp>

class c_player_entity : public c_network_entity
{
public:
	c_player_entity(std::string client_name);

public:
	std::string m_client_name;

	int32_t m_model;
	
	float m_health;
	float m_armor;

	sdk_vec3_t m_position;
	sdk_vec3_t m_rotation;

	sdk_vec3_t m_vec_speed;

	packet_player_sync_data* m_player_sync_data;

public:
	void spawn(sdk_vec3_t position);
};