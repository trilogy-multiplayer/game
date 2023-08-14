#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ENET_IMPLEMENTATION
#define LIBRG_DEBUG

#include <iostream>
#include <string>
#include <sstream>

#include <vendor/librg/zpl/zpl.h>
#include <vendor/librg/zpl/zpl_math.h>
#include <vendor/librg/enet.h>
#include <vendor/librg/librg.h>

#include <utilities/logging.hpp>
#include <definitions.hpp>

#include <thread>
#include <chrono>

librg_ctx_t m_ctx;

static void on_connect_requesting(librg_event_t* event)
{
	c_log::Info("Networking", ">>", "on_connect_requesting");
}

int main()
{

	m_ctx.mode = LIBRG_MODE_CLIENT;
	m_ctx.tick_delay = 32;
	librg_init(&m_ctx);

	librg_event_add(&m_ctx, LIBRG_CONNECTION_REQUEST, on_connect_requesting);

	librg_address_t address = { 1337, (char*)"127.0.0.1" };
	librg_network_start(&m_ctx, address);

	c_log::Info("Networking", ">>", "Connecting to client at port:", address.port);

	while (true)
	{
		librg_tick(&m_ctx);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	librg_network_stop(&m_ctx);
	librg_free(&m_ctx);
}