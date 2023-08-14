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

#define REGISTER_LIBRG_EVENT(context, event_name, function_name)								\
	librg_event_add(context, event_name, [](librg_event_t* _event) { function_name(_event); });	\

class c_server_networking : public c_singleton<c_server_networking> {
private:
	librg_ctx_t m_ctx = {};

public:
	bool m_is_running = true;

	void initialize();

	void on_client_thread();
	void on_connect_request(librg_event_t* librg_event);
	void on_connect_accept(librg_event_t* librg_event);
};

#endif