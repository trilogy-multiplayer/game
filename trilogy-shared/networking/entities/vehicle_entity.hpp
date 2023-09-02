#pragma once

#include <cstdint>
#include <array>
#include <networking/entities/network_entity.hpp>
#include <networking/entities/player_entity.hpp>
#include <definitions.hpp>

#ifdef TRILOGY_CLIENT
#include <memory/memory.hpp>
#include <sdk/sdk_ped.hpp>
#endif

#include <networking/packets/packet_player_sync_data.hpp>

#define GET_TARGET_SYNC_MODULE networking::modules::c_module_vehicle_sync::instance()

class c_vehicle_entity : public c_network_entity
{
public:
	c_vehicle_entity(int32_t network_id, int32_t model_index);

	/**
	  * Network entity inherits here
	  */
	void on_local_client_stream(librg_event* event) override;

	void on_entity_create(librg_event* event) override;
	void on_entity_update(librg_event* event) override;
	void on_entity_remove(librg_event* event) override;

	int32_t m_model_index;
	int32_t m_handle;

	c_player_entity* m_network_owner;
	c_player_entity* m_driver;

	std::array<c_player_entity*, 4> m_passengers = std::array<c_player_entity*, 4>();

	int8_t m_primary_color;
	int8_t m_secondary_color;

	bool m_engine_on;

	sdk_vec3_t m_position = sdk_vec3_t(0, 0, 0);
	sdk_vec3_t m_vec_speed = sdk_vec3_t(0, 0, 0);

	int32_t m_health = 100;

	sdk_vec2_t m_rotation{};
	sdk_vec2_t m_force_power{};

	hid::hid_compressed_mapping m_hid_mapping{};
};