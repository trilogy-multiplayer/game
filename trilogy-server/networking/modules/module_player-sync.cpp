#include "module_player-sync.hpp"

void networking::modules::c_module_player_sync::on_player_connect(librg_message_t* librg_event)
{
	std::string client_name = librg_data_rstring(librg_event->data);

	librg_entity_t* librg_entity = librg_entity_find(librg_event->ctx, librg_event->peer);
	if (!librg_entity) {
		/**
		  * TODO: error handling
		  */
		c_log::Error("Error! Entity not found!");
		return;
	}

	c_player_entity* player = new c_player_entity(librg_entity->id, client_name);
	if (!player) {
		return;
	}

	librg_entity->type = (int)e_entity_types::PLAYER;

	librg_lambda_message_send_to(&c_server_networking::instance()->m_ctx, NETWORK_ACCEPT_CONNECTION, librg_event->peer, [&](librg_data* data) {
		librg_data_wu32(data, player->m_network_id);
	librg_data_wstring(data, player->m_name);

	/**
	  * Here im passing some world values to the local client to synchronize the weather & time.
	  * TODO: add global world class
	  */

	  // Time hours & minutes
	librg_data_wu32(data, 13);
	librg_data_wu32(data, 37);

	// Current weather
	librg_data_wu32(data, 4); // EXTRA_SUNNY
		});

	librg_lambda_message_send_except(&c_server_networking::instance()->m_ctx, NETWORK_PLAYER_CONNECT, librg_event->peer, [&](librg_data* data) {
		librg_data_wu32(data, player->m_network_id);
		librg_data_wstring(data, player->m_name);
		});

	for (auto& player_iterator : m_players)
	{
		if (player_iterator == nullptr) continue;
		if (player_iterator->m_network_id == player->m_network_id) continue;

		librg_lambda_message_send_to(&c_server_networking::instance()->m_ctx, NETWORK_PLAYER_CONNECT, librg_entity->client_peer, [&](librg_data* data) {
			librg_data_wu32(data, player_iterator->m_network_id);
			librg_data_wstring(data, player_iterator->m_name);
			});
	}

	enet_peer_timeout(librg_event->peer, 10, 4000, 4000);

	librg_entity_control_set(librg_event->ctx, librg_entity->id, librg_entity->client_peer);

	player->spawn(
		sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z)
	);
}

void networking::modules::c_module_player_sync::on_player_spawn(librg_message_t* librg_event)
{
	librg_entity_t* librg_entity = librg_entity_find(librg_event->ctx, librg_event->peer);
	if (!librg_entity) {
		return;
	}

	c_player_entity* player = this->m_players.at(librg_entity->id);
	if (!player) {
		return;
	}

	/**
	  * In scripts we want to:
	  * player -> requesting spawn
	  * server -> handling event & spawn player to position x, y, z
	  */

	player->spawn(
		sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z)
	);
}


void networking::modules::c_module_player_sync::on_incoming_stream_entity_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

	c_player_entity* player = this->m_players.at(event->entity->id);
	if (!player) {
		c_log::Error("(networking::modules::c_module_player_sync::update):", c_log::Join(event->entity->id, "/", "unknown"));

		librg_event_reject(event);
		return;
	}

	player->on_entity_update(event);
}

void networking::modules::c_module_player_sync::on_receive_stream_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

	c_player_entity* player = this->m_players.at(event->entity->id);
	if (!player) {
		c_log::Error("(networking::modules::c_module_player_sync::on_receive_stream_update):", c_log::Join(event->entity->id, "/", "unknown"));

		librg_event_reject(event);
		return;
	}

	player->on_local_client_stream(event);
}

void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_PLAYER_CONNECT, networking::modules::c_module_player_sync::instance()->on_player_connect);
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_SPAWN_PLAYER, networking::modules::c_module_player_sync::instance()->on_player_spawn);

	REGISTER_LIBRG_EVENT(librg_context, LIBRG_ENTITY_UPDATE, networking::modules::c_module_player_sync::instance()->on_incoming_stream_entity_update);
	//REGISTER_LIBRG_EVENT(librg_context, LIBRG_ENTITY_REMOVE, networking::modules::c_module_player_sync::instance()->on_receive_stream_update);
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_CLIENT_STREAMER_UPDATE, networking::modules::c_module_player_sync::instance()->on_receive_stream_update);
}


void networking::modules::c_module_player_sync::shutdown()
{
}
