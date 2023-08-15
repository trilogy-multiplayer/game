#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_SERVER_NETWORKING_H
#define TRILOGY_SERVER_NETWORKING_H

#include <common.hpp>

#include <vendor/librg/zpl/zpl.h>
#include <vendor/librg/zpl/zpl_math.h>
#include <vendor/librg/enet.h>
#include <vendor/librg/librg.h>

#include <networking/modules/module_player-sync.hpp>

#define REGISTER_LIBRG_EVENT(context, event_name, function_name) \
	librg_event_add(context, event_name, [](librg_event_t* _event) { function_name(_event); });	\

#define REGISTER_LIBRG_MESSAGE(context, event_name, function_name) \
	librg_network_add(context, event_name, [](librg_message_t * _event) { function_name(_event); }); \

class c_server_networking : public c_singleton<c_server_networking> {
public:
	librg_ctx_t m_ctx = {};

	bool m_is_running = true;
public:

	void initialize();

	void on_client_thread();
	void on_connect_request(librg_event_t* librg_event);
	void on_connect_accept(librg_event_t* librg_event);
};

#endif