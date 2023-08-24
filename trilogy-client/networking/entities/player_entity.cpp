#include "player_entity.hpp"
#include <networking/networking.hpp>
#include <utilities/ida.hpp>

#include <sdk/api/sdk_ped_api.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string name, bool is_local)
{
	this->m_network_id = network_id;

	this->player_id = is_local ? 0 :
		networking::modules::c_module_player_sync::instance()->m_free_id++;

	this->m_entity_type = e_entity_types::PLAYER;
	this->m_name = name;

	/**
	  * Set the char id only if player is local player, because:
	  * local player will be the same character all the time
	  */
	if (is_local)
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, SDK_LOCAL_PLAYER, &this->char_id);

	/**
	  * Maybe replace this with member variables in player entity class.
	  */
	this->player_sync_data = new packet_player_sync_data();

	networking::modules::c_module_player_sync::instance()->m_players.at(network_id) = this;
}

void c_player_entity::on_local_client_stream(librg_event* event) {
	/**
	  * lol basically this is useless
	  * this == player?
	  */
	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(m_network_id);
	if (player == nullptr) {
		c_log::Error(c_log::LRed, "(c_player_entity::on_client_stream):",
			c_log::LWhite, "Unable to find local player.", m_network_id, player->char_id);
		return;
	}

	static auto ped_api = sdk::api::sdk_ped_api::instance();

	sdk_ped* game_player = c_memory::instance()->sdk_find_player_ped(SDK_LOCAL_PLAYER);
	if (!game_player) return;

	if (!m_game_player)
		m_game_player = game_player;

	int health;
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_CHAR_HEALTH, player->char_id, &health);

	int armor;
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_CHAR_ARMOUR, player->char_id, &armor);

	/**
	  * Event is equal to last synced data.
	  * Reject it.
	  */
	if (player->m_position == game_player->m_matrix->m_position &&
		player->m_vec_speed == game_player->m_vec_speed &&
		player->m_health == health &&
		player->m_armor == armor) {
		librg_event_reject(event);
		return;
	}

	player->m_position = game_player->m_matrix->m_position;
	player->m_vec_speed = game_player->m_vec_speed;

	player->m_rotation = ped_api->get_rotation(game_player);
	player->m_force_power = ped_api->get_force_power(game_player);
	player->m_camera_front = c_memory::instance()->sdk_current_camera_data_front->m_front_pos;

	player->m_health = health;
	player->m_armor = armor;

	player->m_hid_mapping = hid::compress_mapping(*c_memory::instance()->sdk_hid_mapping);

	/**
	  * Stream the local position through to interact with the stream-in & stream-out events.
	  */
	event->entity->position = player->m_position;

	librg_data_wptr(event->data, &player->m_position, sizeof(sdk_vec3_t));
	librg_data_wptr(event->data, &player->m_vec_speed, sizeof(sdk_vec3_t));
	librg_data_wptr(event->data, &player->m_rotation, sizeof(sdk_vec2_t));
	librg_data_wptr(event->data, &player->m_force_power, sizeof(sdk_vec2_t));
	librg_data_wptr(event->data, &player->m_camera_front, sizeof(sdk_vec3_t));

	librg_data_wu32(event->data, player->m_health);
	librg_data_wu32(event->data, player->m_armor);

	librg_data_wptr(event->data, &player->m_hid_mapping, sizeof(hid::hid_compressed_mapping));
}

void c_player_entity::on_entity_create(librg_event* event)
{
	static auto memory = c_memory::instance();

	int temporary_player_handle;
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_CREATE_PLAYER, SDK_REMOTE_PLAYER,
		event->entity->position.x, event->entity->position.y, event->entity->position.z, &temporary_player_handle);

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, temporary_player_handle, &this->char_id);

	m_game_player = memory->sdk_world_players[56 * temporary_player_handle];
	if (m_game_player == nullptr) {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_DELETE_CHAR, this->char_id);

		librg_event_reject(event);
		return;
	}

	memory->sdk_world_players[56 * temporary_player_handle] = nullptr;
}

void c_player_entity::on_entity_update(librg_event* event)
{
	static auto ped_api = sdk::api::sdk_ped_api::instance();

	librg_data_rptr(event->data, &m_position, sizeof(sdk_vec3_t));
	librg_data_rptr(event->data, &m_vec_speed, sizeof(sdk_vec3_t));
	librg_data_rptr(event->data, &m_rotation, sizeof(sdk_vec2_t));
	librg_data_rptr(event->data, &m_force_power, sizeof(sdk_vec2_t));
	librg_data_rptr(event->data, &m_camera_front, sizeof(sdk_vec3_t));

	m_health = librg_data_ru32(event->data);
	m_armor = librg_data_ru32(event->data);

	librg_data_rptr(event->data, &m_hid_mapping, sizeof(hid::hid_compressed_mapping));

	/**
	  * TODO:
	  * We need to adjust the positions in the process-control hook later.
	  * Might delete this part
	  */

	  // Adjust the game player values with the player class

	// c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CHAR_HEALTH, char_id, m_health);
	// c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CHAR_ARMOUR, char_id, m_armor);
}

void c_player_entity::on_entity_remove(librg_event* event)
{
	this->use_player_context(m_game_player, [&] {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_DELETE_PLAYER, SDK_CONTEXT_PLAYER);

		this->char_id = -1;
		this->m_game_player = nullptr;
	});
}
