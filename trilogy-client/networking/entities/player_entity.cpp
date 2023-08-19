#include "player_entity.hpp"
#include <networking/networking.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string name, bool is_local)
{
	this->m_network_id = network_id;
	this->player_id = networking::modules::c_module_player_sync::instance()->m_free_id++;

	this->m_entity_type = e_entity_types::PLAYER;
	this->m_name = name;

	/**
	  * Maybe replace this with member variables in player entity class.
	  */
	this->player_sync_data = new packet_player_sync_data();

	networking::modules::c_module_player_sync::instance()->m_players.at(network_id) = this;
}


void c_player_entity::on_client_stream(librg_event* event) {
	auto player = networking::modules::c_module_player_sync::instance()->m_players.at(0);
	if (player == nullptr) return;
}