#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_MODULE_SYNC_H
#define TRILOGY_NETWORKING_MODULE_SYNC_H

#include <common.hpp>
#include <networking/server_networking.hpp>

namespace networking::modules {
	class c_module_sync {
	public:
		virtual e_entity_types get_sync_type() { return e_entity_types::EMPTY; }
		// virtual c_network_entity get_sync_entity() { return c_network_entity; }

		virtual void initialize(librg_ctx* librg_context) { }
		virtual void shutdown() { }

		virtual void on_receive_stream_update(librg_event_t* librg_event) { }
	};
}
#endif