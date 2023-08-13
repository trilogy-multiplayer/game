#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_H
#define TRILOGY_NETWORKING_H

#define WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <thread>

#include <vendor/librg/zpl/zpl.h>
#include <vendor/librg/zpl/zpl_math.h>
#include <vendor/librg/enet.h>
#include <vendor/librg/librg.h>

#include <utilities/logging.hpp>
#include "utilities/singleton.hpp"

#include <networking/entities/player_entity.hpp>
#include <networking/modules/module_player-sync.hpp>

#include <vendor/minhook/minhook.hpp>
#include <utilities/ida.hpp>
#include <mutex>

#define REGISTER_LIBRG_EVENT(context, event_name, function_name)					\
	auto callback__##event_name = ([&](librg_event_t* librg_event) {				\
		this->function_name(librg_event);											\
	});																				\
	librg_event_add(context, event_name, (librg_event_cb*)&callback__##event_name);	\

class c_networking : public c_singleton<c_networking> {
public:
	librg_ctx_t m_ctx;

	bool m_is_running = false;
	bool m_is_connected = false;

	int32_t player_id = 1;

	using sdk_player_connect_t = void(*)(int64_t this_ptr);
	sdk_player_connect_t o_sdk_player_connect;

	c_player_entity* get_player_by_id(int32_t id);
	c_player_entity* get_player_by_game_id(int32_t game_id);
	std::vector<c_player_entity*> m_players;

	librg_address_t m_address;
	std::string m_client_name = "trilogy-admin";

	void initialize();
	bool connect_to(const char* address, int32_t port);

	void on_client_thread();
	void on_connect_request(librg_event_t* librg_event);
	void on_connect_accept(librg_event_t* librg_event);
};

#endif