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
	private:
		virtual e_entity_types get_sync_type() override { return e_entity_types::PLAYER; }

		std::vector<c_player_entity> m_players;
	public:
		void initialize(librg_ctx* librg_context) override;
		void shutdown() override;

		void on_local_stream_update(librg_event_t* librg_event) override;

	};
}
#endif