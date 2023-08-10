#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ENET_IMPLEMENTATION
#define LIBRG_DEBUG

#include <iostream>

#include <vendor/librg/zpl/zpl.h>
#include <vendor/librg/zpl/zpl_math.h>
#include <vendor/librg/enet.h>
#include <vendor/librg/librg.h>

#include <utilities/logging.hpp>
#include <definitions.hpp>

#include <thread>
#include <chrono>
#include "networking/entities/server_entity.hpp"

librg_ctx_t m_ctx;
std::vector<c_server_entity*> players = {};

static void on_connect_requesting(librg_event_t* event)
{
	c_log::Info("Networking", ">>", "Connecting from", librg_data_rstr(event->data));
}

void on_connect_accepted(librg_event_t* event)
{
	event->entity->type = e_entity_types::PLAYER;

	auto player = new c_server_entity(event->entity->id, "unknown", event->entity->client_peer);
	player->player_sync_data = new packet_player_sync_data();
	players.push_back(player);

	event->entity->user_data = player;

	librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);

	c_log::Info("Networking", ">>", "on_connect_accepted");
}


void on_entity_update(librg_event_t* event)
{

	if (!event->entity->user_data)
		return;

	if (event->entity->type == e_entity_types::PLAYER)
	{
	}
}

void on_entity_remove(librg_event_t* event)
{

}

void on_stream_update(librg_event_t* event)
{
	if (!event->entity->user_data)
		return;

	if (event->entity->type == e_entity_types::PLAYER) {
		c_server_entity* player = (c_server_entity*)event->entity->user_data;

		//c_log::Info("Incoming sync from:", player->network_id);

		//librg_data_rptr(event->data, &player->player_sync_data, sizeof(packet_player_sync_data));

		//auto onfoot = player->player_sync_data->controls.onFootKeys;

		//c_log::Info("Received sync", onfoot.bJump == true);
	}
	// c_log::Info("Received sync", player->player_sync_data->controls.onFootKeys.bJump, player->player_sync_data->controls.onFootKeys.bSprint);
	// librg_data_rptr(event->data, player->syncData, size);
}


int main()
{
	m_ctx.world_size = zpl_vec3f(5000.0f, 5000.0f, 5000.0f);
	m_ctx.mode = LIBRG_MODE_SERVER;
	m_ctx.tick_delay = 16;
	m_ctx.max_connections = (MAX_PLAYERS * 2);
	m_ctx.max_entities = (MAX_ENTITIES + MAX_PLAYERS);
	librg_init(&m_ctx);

	//librg_option_set(LIBRG_DEFAULT_STREAM_RANGE, 10);
	librg_event_add(&m_ctx, LIBRG_CONNECTION_REQUEST, on_connect_requesting);
	librg_event_add(&m_ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

	librg_event_add(&m_ctx, LIBRG_ENTITY_UPDATE, on_entity_update);
	librg_event_add(&m_ctx, LIBRG_ENTITY_REMOVE, on_entity_remove);

	librg_event_add(&m_ctx, LIBRG_CLIENT_STREAMER_UPDATE, on_stream_update);

	c_log::Info("Networking", ">>", "Initialized server context.");

	librg_address_t address = { 1337 };
	librg_network_start(&m_ctx, address);

	c_log::Info("Networking", ">>", "Started server @::", address.port);

	while (true)
	{
		librg_tick(&m_ctx);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	librg_network_stop(&m_ctx);
	librg_free(&m_ctx);
}