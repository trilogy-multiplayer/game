#pragma once

#include <cstdint>
#include <networking/entities/network_entity.hpp>
#include <definitions.hpp>

#include <networking/packets/packet_player_sync_data.hpp>

class c_player_entity : public c_network_entity
{
public:
	c_player_entity(int32_t network_id, std::string client_name);

public:
	std::string m_client_name;

	int32_t m_model;

	/**
	 * TODO:
	 * I think it would be better if we split this shit up into 1 shared header
	 * & seperate exclusive server & client data into another header e.g.: server_player_entity
	 */
	sdk_vec3_t m_position = sdk_vec3_t(0, 0, 0);
	sdk_vec3_t m_vec_speed = sdk_vec3_t(0, 0, 0);

	int m_health = 100;
	int m_armor = 0;

	sdk_vec2_t m_rotation = sdk_vec2_t(0, 0);
	sdk_vec2_t m_force_power = sdk_vec2_t(0, 0);
	sdk_vec3_t m_camera_front = sdk_vec3_t(0, 0, 0);

	hid::hid_compressed_mapping m_hid_mapping;

	packet_player_sync_data* m_player_sync_data;

public:
	void spawn(sdk_vec3_t position);
};