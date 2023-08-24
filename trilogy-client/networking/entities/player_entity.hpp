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
	c_player_entity(int32_t network_id, std::string name, bool is_local = false);

	/**
	  * Network entity inherits here
	  */
	void on_local_client_stream(librg_event* event) override;

	void on_entity_create(librg_event* event) override;
	void on_entity_update(librg_event* event) override;
	void on_entity_remove(librg_event* event) override;

	sdk_ped* m_game_player;

	int char_id = -1;
	int player_id = 0;

	bool is_local;

	std::string m_name = "";

	sdk_vec3_t m_position = sdk_vec3_t(0, 0, 0);
	sdk_vec3_t m_vec_speed = sdk_vec3_t(0, 0, 0);

	int m_health = 100;
	int m_armor = 0;

	sdk_vec2_t m_rotation = sdk_vec2_t(0, 0);
	sdk_vec2_t m_force_power = sdk_vec2_t(0, 0);
	sdk_vec3_t m_camera_front;

	hid::hid_compressed_mapping m_hid_mapping{};

	// Can we delete this?
	packet_player_sync_data* player_sync_data;

public:
	void use_player_context(sdk_ped* context_player, std::function<void()> f) {
		static auto memory = c_memory::instance();
		
		memory->sdk_world_players[56 * SDK_CONTEXT_PLAYER] = context_player;
		
		f();
		
		memory->sdk_world_players[56 * SDK_CONTEXT_PLAYER] = nullptr;
	}
};