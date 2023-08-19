#include "module_player-sync.hpp"

void networking::modules::c_module_player_sync::on_player_connect(librg_message_t* event)
{
	int32_t network_id = librg_data_ru32(event->data);
	std::string client_name = librg_data_rstring(event->data);

	c_player_entity* player = new c_player_entity(network_id, client_name);

	c_log::Info("Player connected:", player->m_name);
}


void networking::modules::c_module_player_sync::on_player_spawn(librg_message_t* event)
{
	sdk_vec3_t position;
	librg_data_rptr(event->data, &position, sizeof(sdk_vec3_t));

	auto player_ped = c_memory::instance()->sdk_find_player_ped(0);
	if (player_ped == nullptr) return;

	/**
	  * Change sdk_ped class, switch pos_x, pos_y, pos_z to sdk_vec3_t
	  */
	player_ped->m_matrix->pos_x = position.x;
	player_ped->m_matrix->pos_y = position.y;
	player_ped->m_matrix->pos_z = position.z;
}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_create(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	c_log::Error("(networking::modules::c_module_player_sync::on_incoming_stream_entity_create):", librg_event->entity->id, librg_event->entity->type);
}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_remove(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	c_log::Error("(networking::modules::c_module_player_sync::on_incoming_stream_entity_remove):", librg_event->entity->id, librg_event->entity->type);

}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_update(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	c_log::Error("(networking::modules::c_module_player_sync::on_incoming_stream_entity_update):", librg_event->entity->id, librg_event->entity->type);
}

void networking::modules::c_module_player_sync::on_local_stream_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

}

void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_PLAYER_CONNECT, networking::modules::c_module_player_sync::instance()->on_player_connect);
	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_SPAWN_PLAYER, networking::modules::c_module_player_sync::instance()->on_player_spawn);

	REGISTER_LIBRG_EVENT(librg_context, LIBRG_ENTITY_CREATE, networking::modules::c_module_player_sync::instance()->on_incoming_stream_entity_create);
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_ENTITY_REMOVE, networking::modules::c_module_player_sync::instance()->on_incoming_stream_entity_remove);
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_ENTITY_UPDATE, networking::modules::c_module_player_sync::instance()->on_incoming_stream_entity_update);

	REGISTER_LIBRG_EVENT(librg_context, LIBRG_CLIENT_STREAMER_UPDATE, networking::modules::c_module_player_sync::instance()->on_local_stream_update);
}


void networking::modules::c_module_player_sync::shutdown()
{
}
