#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_MODULE_VEHICLESYNC_H
#define TRILOGY_NETWORKING_MODULE_VEHICLESYNC_H

#include <common.hpp>
#include <networking/server_networking.hpp>

#include <networking/modules/module_sync.hpp>
#include <networking/entities/vehicle_entity.hpp>

namespace networking::modules {
	class c_module_vehicle_sync : public c_singleton<c_module_vehicle_sync>, c_module_sync {
	public:
		virtual e_entity_types get_sync_type() override { return e_entity_types::VEHICLE; }

		std::vector<c_vehicle_entity*> m_vehicles = std::vector<c_vehicle_entity*>(MAX_VEHICLES);
		int32_t m_free_id = 1;
	public:
		void initialize(librg_ctx* librg_context) override;
		void shutdown() override;

		void on_player_connect(librg_message_t* librg_event);
		void on_player_spawn(librg_message_t* librg_event);

		void on_receive_stream_update(librg_event_t* librg_event) override;
		void on_incoming_stream_entity_update(librg_event_t* librg_event) override;
	};
}
#endif