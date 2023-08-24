#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_MODULE_PLAYERSYNC_H
#define TRILOGY_NETWORKING_MODULE_PLAYERSYNC_H

#include <common.hpp>
#include <networking/networking.hpp>
#include <networking/modules/module_sync.hpp>

namespace networking::modules {
	class c_module_player_sync : public c_singleton<c_module_player_sync>, c_module_sync {
	public:
		virtual e_entity_types get_sync_type() override { return e_entity_types::PLAYER; }

		std::vector<c_player_entity*> m_players = std::vector<c_player_entity*>(MAX_PLAYERS);
		std::vector<c_player_entity*> m_streamed_players = std::vector<c_player_entity*>();

		int32_t m_free_id = 0;

		c_player_entity* m_local_player;

	public:
		using process_control_t = void(*)(sdk_ped* this_ptr);
		process_control_t o_process_control;

	public:
		void initialize(librg_ctx* librg_context) override;
		void shutdown() override;

		void on_local_accept_connection(librg_message_t* librg_event);

		void on_player_connect(librg_message_t* librg_event);
		void on_player_spawn(librg_message_t* librg_event);

		void on_incoming_stream_entity_create(librg_event_t* librg_event) override;
		void on_incoming_stream_entity_remove(librg_event_t* librg_event) override;
		void on_incoming_stream_entity_update(librg_event_t* librg_event) override;

		void on_local_stream_update(librg_event_t* librg_event) override;

	public:
		c_player_entity* get_player_by_ptr(int64_t this_ptr);
	};
}
#endif