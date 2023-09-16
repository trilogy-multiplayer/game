#include "module_player-sync.hpp"

#include <sdk/sdk_weathers.hpp>
#include <sdk/api/sdk_ped_api.hpp>

#include <renderer/features/feature_dev-chat.hpp>

void h_process_control(sdk_ped* this_ptr)
{
	auto module_player_sync = networking::modules::c_module_player_sync::instance();
	auto ped_api = sdk::api::sdk_ped_api::instance();

	if (this_ptr == c_memory::instance()->sdk_find_player_ped(SDK_LOCAL_PLAYER)) {
		module_player_sync->o_process_control(this_ptr);
		return;
	}

	auto player = module_player_sync->get_player_by_ptr(
		reinterpret_cast<int64_t>(this_ptr));

	if (player == nullptr) return;

	hid::hid_mapping current_hid_state = *c_memory::instance()->sdk_hid_mapping;
	auto current_camera_data_front = *c_memory::instance()->sdk_current_camera_data_front;

	*c_memory::instance()->sdk_hid_mapping = hid::decompress_mapping(player->m_hid_mapping, current_hid_state);
	c_memory::instance()->sdk_current_camera_data_front->m_front_pos = player->m_camera_front;

	ped_api->set_rotation(player->m_game_player, player->m_rotation);
	ped_api->set_force_power(player->m_game_player, player->m_force_power);

	// Workaround for this, maybe move into player_entity
	player->m_game_player->update_position(player->m_position, player->m_vec_speed);

	module_player_sync->o_process_control(this_ptr);

	*c_memory::instance()->sdk_current_camera_data_front = current_camera_data_front;
	*c_memory::instance()->sdk_hid_mapping = current_hid_state;
}

void h_set_task_unknown(int64_t this_ptr, int64_t* unk)
{
	auto module_player_sync = networking::modules::c_module_player_sync::instance();
	module_player_sync->o_set_task_unknown(this_ptr, unk);

	c_log::Debug(c_log::LRed, "(c_module_player_sync::h_set_task_unknown):",
		c_log::LWhite, "Task", c_log::Cyan, this_ptr, unk);
}

void networking::modules::c_module_player_sync::on_local_accept_connection(librg_message_t* librg_event)
{
	int32_t network_id = librg_data_ru32(librg_event->data);
	std::string local_name = librg_data_rstring(librg_event->data);

	int32_t time_hours = librg_data_ru32(librg_event->data);
	int32_t time_minutes = librg_data_ru32(librg_event->data);

	e_sdk_weathers weather = (e_sdk_weathers)librg_data_ru32(librg_event->data);

	m_local_player = new c_player_entity(network_id, local_name, true);

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_TIME_OF_DAY, time_hours, time_minutes);
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_FORCE_WEATHER_NOW, (int32_t)weather);

	c_log::Error(c_log::LRed, "(c_module_player_sync::on_local_accept_connection):",
		c_log::LWhite, "Joined the server as", c_log::Cyan, m_local_player->m_name, "-", m_local_player->m_network_id);
}

void networking::modules::c_module_player_sync::on_player_connect(librg_message_t* event)
{
	int32_t network_id = librg_data_ru32(event->data);
	std::string client_name = librg_data_rstring(event->data);

	c_player_entity* player = new c_player_entity(network_id, client_name);

	// renderer::features::c_dev_chat::instance()->m_chat_messages.push_back({ "(on_player_connect):", c_log::Join("Player", player->m_name, "joined the server."), 3000 });
}

void networking::modules::c_module_player_sync::on_player_spawn(librg_message_t* event)
{
	int32_t network_id = librg_data_ru32(event->data);

	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(network_id);
	if (player == nullptr) return;

	sdk_vec3_t position;
	librg_data_rptr(event->data, &position, sizeof(sdk_vec3_t));

	int32_t model = librg_data_ru32(event->data);

	/**
	  * Currently this is getting called before Entity-Creation
	  * Workaround!
	  */
	if (player->m_game_player == nullptr) {
		c_log::Info("Game player is null");
		return;
	}

	memory::features::c_model_resolver::instance()->add_model_to_worker(model, [player](int32_t model_index)
		{
			player->use_player_context([model_index]
				{
					c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_PLAYER_MODEL, SDK_CONTEXT_PLAYER, model_index);
				});
		});

	// sdk::api::sdk_ped_api
	// c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_PLAYER_MODEL, model_index);

	/**
	  * Change sdk_ped class, switch pos_x, pos_y, pos_z to sdk_vec3_t
	  * EDIT: done.
	  */

	player->m_position = position;
	player->m_game_player->m_matrix->m_position = position;
}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_create(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(librg_event->entity->id);
	if (player == nullptr || player->m_is_local) return;

	/**
	  * TODO:
	  * Add player pointer implementation to player_entity
	  */
	if (player->m_char_id != -1) {
		// The player already exists -> theres something fucked up if this fires
		c_log::Info("something fucked up");
		librg_event_reject(librg_event);
		return;
	}

	player->on_entity_create(librg_event);

	this->m_streamed_players.push_back(player);
}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_remove(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(librg_event->entity->id);
	if (player == nullptr || player->m_is_local) return;

	/**
	  * TODO:
	  * Add player pointer implementation to player_entity
	  */
	if (player->m_char_id == -1) {
		// The player already exists -> theres something fucked up if this fires
		c_log::Info("something fucked up");
		librg_event_reject(librg_event);
		return;
	}

	player->on_entity_remove(librg_event);

	auto& streamed_players = this->m_streamed_players;

	this->m_streamed_players.erase(
		std::remove(streamed_players.begin(), streamed_players.end(), player), streamed_players.end());
}

void networking::modules::c_module_player_sync::on_incoming_stream_entity_update(librg_event_t* librg_event)
{
	if (librg_event->entity->type != (int32_t)this->get_sync_type()) return;

	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(librg_event->entity->id);
	if (player == nullptr || player->m_is_local) return;

	if (player->m_game_player == nullptr) {
		librg_event_reject(librg_event);
		return;
	}

	player->on_entity_update(librg_event);
}

void networking::modules::c_module_player_sync::on_local_stream_update(librg_event_t* event)
{
	if (event->entity->id != m_local_player->m_network_id) {
		c_log::Error(c_log::LRed, "(networking::modules::c_module_player_sync::on_local_stream_update):",
			c_log::LWhite, "wtf?! why is someone else trying stream another player???");
		return;
	}

	/**
	  * Just stream it?
	  * I would say add more checks if someone is nearby him too save some server performance
	  */
	m_local_player->on_local_client_stream(event);
}

c_player_entity* networking::modules::c_module_player_sync::get_player_by_ptr(int64_t this_ptr)
{
	for (auto& player : this->m_streamed_players)
	{
		if (player == nullptr) continue;
		if (player->m_game_player == nullptr) continue;

		if (reinterpret_cast<int64_t>(player->m_game_player) == this_ptr)
			return player;
	}

	return nullptr;
}

void networking::modules::c_module_player_sync::initialize(librg_ctx* librg_context)
{
	auto process_control = memory::find_pattern<process_control_t>(memory::module_t(nullptr), "networking::modules::c_module_player_sync::process_control",
		"48 8B C4 48 89 58 20 55 56 57 41 54 41 55 48 8D 68 A1");

	MH_CreateHook(process_control, h_process_control, reinterpret_cast<void**>(&o_process_control));
	MH_EnableHook(process_control);


	auto set_task_unknown = memory::find_pattern<set_task_unknown_t>(memory::module_t(nullptr), "networking::modules::c_module_player_sync::set_task_unknown",
		"48 89 5C 24 10 57 48 83 EC ? 48 8D 79 08 48 8B DA 48 85 D2 75 ? 48 8B 4F 20");

	MH_CreateHook(set_task_unknown, h_set_task_unknown, reinterpret_cast<void**>(&o_set_task_unknown));
	MH_EnableHook(set_task_unknown);

	REGISTER_LIBRG_MESSAGE(librg_context, NETWORK_ACCEPT_CONNECTION, networking::modules::c_module_player_sync::instance()->on_local_accept_connection);
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
