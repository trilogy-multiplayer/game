#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_MODULE_SYNC_H
#define TRILOGY_NETWORKING_MODULE_SYNC_H

#include <common.hpp>
#include <networking/networking.hpp>

namespace networking::modules {
	class c_module_sync {
	public:
		virtual e_entity_types get_sync_type() { return e_entity_types::EMPTY; }
		// virtual c_network_entity get_sync_entity() { return c_network_entity; }

		virtual void initialize(librg_ctx* librg_context) { }
		virtual void shutdown() { }

		virtual void on_incoming_stream_entity_create(librg_event_t* librg_event) { }
		virtual void on_incoming_stream_entity_remove(librg_event_t* librg_event) { }
		virtual void on_incoming_stream_entity_update(librg_event_t* librg_event) { }

		// Cant override it because func needs to be static
		virtual void on_local_stream_update(librg_event_t* librg_event) { }
	};
}
#endif