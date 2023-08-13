#include "player_entity.hpp"
#include <networking/networking.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string name, bool is_local, sdk_vec3_t pos)
{
	c_networking::instance()->player_id++;

	this->network_id = network_id;
	this->player_id = c_networking::instance()->player_id;

	if (is_local) {
		// this->ped = c_memory::instance()->sdk_ped_pool->GetAt(0);
		this->player_id = 0;
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &this->char_id);

		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CHAR_COORDINATES,
			this->char_id,
			pos.x, pos.y, pos.z
		);

		this->is_local = true;
	}
	else {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_CREATE_PLAYER, this->player_id, pos.x, pos.y, pos.z, &this->player_id);
	}

	// auto ped_ptr = *(_QWORD*)*c_memory::instance()->sdk_ped_pool + *c_memory::instance()->sdk_ped_pool_padding * ((__int64)this->char_id >> 8);
	// auto ped = (sdk_ped*)ped_ptr;

	this->entity_type = e_entity_types::PLAYER;
	this->name = name;
	this->player_sync_data = new packet_player_sync_data();
	this->player_sync_data->name = name.c_str();

	c_networking::instance()->m_players.push_back(this);
}

/*
void c_player_entity::on_client_stream(librg_event* event) {

	c_player_entity* player = (c_player_entity*)event->entity->user_data;
	sdk_ped* player_ped = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(0);

	if (player == nullptr || player_ped == nullptr) return;

	player->player_sync_data = new packet_player_sync_data();
	player->player_sync_data->name = c_networking::instance()->m_client_name.c_str();
	player->player_sync_data->mapping = compress_mapping(*c_memory::instance()->sdk_hid_mapping);
	player->player_sync_data->move_speed = sdk_vec3_t(player_ped->m_vec_speed_x, player_ped->m_vec_speed_y, player_ped->m_vec_speed_z);
	player->player_sync_data->camera_front = c_memory::instance()->sdk_current_camera_data_front->get_offset_pos();

	player->player_sync_data->ped_state = *(_DWORD*)((__int64)player_ped + 1428);
	player->player_sync_data->current_rotation = *(float*)((__int64)player_ped + 1820);

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_CHAR_COORDINATES, player->char_id,
		&event->entity->position.x, &event->entity->position.y, &event->entity->position.z);

	librg_data_wptr(event->data, player->player_sync_data, sizeof(packet_player_sync_data));

}*/