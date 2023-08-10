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

class c_networking : public c_singleton<c_networking> {
private:
	librg_ctx_t m_ctx;

	bool m_is_running = false;
	bool m_is_connected = false;

public:
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
};

#endif