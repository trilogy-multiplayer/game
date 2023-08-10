#include "player_entity.hpp"
#include <networking/networking.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string name, bool is_local, sdk_vec3_t pos)
{
	c_networking::instance()->player_id++;

	this->network_id = network_id;
	
	if (is_local) {
		// this->ped = c_memory::instance()->sdk_ped_pool->GetAt(0);
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &this->char_id);

		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CHAR_COORDINATES,
			this->char_id,
			pos.x, pos.y, pos.z
		);

		this->is_local = true;
	}
	else {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_CREATE_CHAR, 4, 0, pos.x, pos.y, pos.z, &this->char_id);
	}

	auto ped_ptr = *(_QWORD*)*c_memory::instance()->sdk_ped_pool + *c_memory::instance()->sdk_ped_pool_padding * ((__int64)this->char_id >> 8);
	auto ped = (sdk_ped*)ped_ptr;

	this->ped = ped;

	this->entity_type = e_entity_types::PLAYER;
	this->name = name;

	c_networking::instance()->m_players.push_back(this);
}
