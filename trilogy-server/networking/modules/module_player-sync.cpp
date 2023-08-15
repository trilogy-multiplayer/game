#include "module_player-sync.hpp"

void networking::modules::c_module_player_sync::on_player_connect(librg_message_t* librg_event)
{
	std::string client_name = librg_data_rstring(librg_event->data);

	librg_entity_t* librg_entity = librg_entity_find(librg_event->ctx, librg_event->peer);
	if (!librg_entity) {
		return;
	}

	c_player_entity* player = new c_player_entity(client_name);
	if (!player) {
		return;
	}

	librg_data data; 
	librg_data_init(&data); 
	
	librg_data_wu32(&data, player->network_id);
	librg_data_wstring(&data, player->m_client_name);
	librg_message_send_except(&c_server_networking::instance()->m_ctx, NETWORK_PLAYER_CONNECT, librg_event->peer, data.rawptr, librg_data_get_wpos(&data));
	librg_data_free(&data);
}

void networking::modules::c_module_player_sync::on_receive_stream_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

	c_player_entity* player = this->m_players.at(event->entity->id);
	if (!player) {
		librg_event_reject(event);
		return;
	}

	librg_data_rptr(event->data, &player->m_position, sizeof(sdk_vec3_t));
}

void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_PLAYER_CONNECT, networking::modules::c_module_player_sync::instance()->on_player_connect);
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_CLIENT_STREAMER_UPDATE, networking::modules::c_module_player_sync::instance()->on_receive_stream_update);
}


void networking::modules::c_module_player_sync::shutdown()
{
}
