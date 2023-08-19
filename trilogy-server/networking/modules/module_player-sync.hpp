#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_MODULE_PLAYERSYNC_H
#define TRILOGY_NETWORKING_MODULE_PLAYERSYNC_H

#include <common.hpp>
#include <networking/server_networking.hpp>
#include <networking/modules/module_sync.hpp>
#include <networking/entities/player_entity.hpp>

namespace networking::modules {
	class c_module_player_sync : public c_singleton<c_module_player_sync>, c_module_sync {
	public:
		virtual e_entity_types get_sync_type() override { return e_entity_types::PLAYER; }

		std::vector<c_player_entity*> m_players = std::vector<c_player_entity*>(MAX_PLAYERS);
		int32_t m_free_id = 0;
	public:
		void initialize(librg_ctx* librg_context) override;
		void shutdown() override;

		void on_player_connect(librg_message_t* librg_event);
		void on_player_spawn(librg_message_t* librg_event);

		void on_receive_stream_update(librg_event_t* librg_event) override;

	};
}
#endif