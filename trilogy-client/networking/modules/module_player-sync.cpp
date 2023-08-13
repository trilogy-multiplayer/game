#include "module_player-sync.hpp"

void networking::modules::c_module_player_sync::on_local_stream_update(librg_event_t* event)
{
	if (event->entity->type != (int32_t)this->get_sync_type()) return;

}


void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	REGISTER_LIBRG_EVENT(librg_context, LIBRG_CLIENT_STREAMER_UPDATE, on_local_stream_update);
}


void networking::modules::c_module_player_sync::shutdown()
{
}
