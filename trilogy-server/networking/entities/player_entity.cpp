#include "player_entity.hpp"

#include <networking/server_networking.hpp>
#include <networking/modules/module_player-sync.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(std::string client_name)
{
	this->network_id = networking::modules::c_module_player_sync::instance()->m_free_id++;

	// auto ped_ptr = *(_QWORD*)*c_memory::instance()->sdk_ped_pool + *c_memory::instance()->sdk_ped_pool_padding * ((__int64)this->char_id >> 8);
	// auto ped = (sdk_ped*)ped_ptr;

	this->entity_type = e_entity_types::PLAYER;
	this->m_client_name = client_name;

	networking::modules::c_module_player_sync::instance()->m_players.at(network_id) = this;
}