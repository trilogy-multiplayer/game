#pragma once

#include <cstdint>
#include <networking/entities/network_entity.hpp>
#include <definitions.hpp>

#ifdef TRILOGY_CLIENT
#include <memory/memory.hpp>
#include <sdk/sdk_ped.hpp>
#endif

#include <networking/packets/packet_player_sync_data.hpp>

#define GET_TARGET_SYNC_MODULE networking::modules::c_module_player_sync::instance()

class c_player_entity : public c_network_entity
{
public:
#ifdef TRILOGY_CLIENT
	c_player_entity(int32_t network_id, std::string name, bool is_local = false);
#else
	c_player_entity(int32_t network_id, std::string client_name);
#endif

	/**
	  * Network entity inherits here
	  */
	void on_local_client_stream(librg_event* event) override;

	void on_entity_create(librg_event* event) override;
	void on_entity_update(librg_event* event) override;
	void on_entity_remove(librg_event* event) override;

	int m_char_id = -1;
	int m_player_id = 0;

	bool m_is_local;

	std::string m_name{};
	int32_t m_model_index;

	sdk_vec3_t m_position = sdk_vec3_t(0, 0, 0);
	sdk_vec3_t m_vec_speed = sdk_vec3_t(0, 0, 0);

	float m_heading;
	float m_quaternion;

	int m_health = 100;
	int m_armor = 0;

	sdk_vec2_t m_rotation{};
	sdk_vec2_t m_force_power{};
	sdk_vec3_t m_camera_front{};

	hid::hid_compressed_mapping m_hid_mapping{};

#ifdef TRILOGY_CLIENT
	sdk_ped* m_game_player;

public:
	void use_player_context(std::function<void()> f) {
		static auto memory = c_memory::instance();

		memory->sdk_world_players[56 * SDK_CONTEXT_PLAYER] = m_game_player;

		f();

		memory->sdk_world_players[56 * SDK_CONTEXT_PLAYER] = nullptr;
	}
#else
public:
	void spawn(sdk_vec3_t position, int32_t model_index = 0);
#endif

};