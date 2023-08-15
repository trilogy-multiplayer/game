#include "module_player-sync.hpp"

void networking::modules::c_module_player_sync::on_player_connect(librg_message_t* event)
{
	int32_t network_id = librg_data_ru32(event->data);
	std::string client_name = librg_data_rstring(event->data);

	c_player_entity* player = new c_player_entity(network_id, client_name);

	c_log::Info("Player connected:", player->m_name);
}

void networking::modules::c_module_player_sync::on_local_stream_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

}

void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_PLAYER_CONNECT, networking::modules::c_module_player_sync::instance()->on_player_connect);
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_CLIENT_STREAMER_UPDATE, networking::modules::c_module_player_sync::instance()->on_local_stream_update);
}


void networking::modules::c_module_player_sync::shutdown()
{
}
