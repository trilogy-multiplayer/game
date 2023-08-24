#include "player_entity.hpp"

#include <networking/server_networking.hpp>
#include <networking/modules/module_player-sync.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string client_name)
{
	this->m_network_id = network_id;

	this->m_entity_type = e_entity_types::PLAYER;
	this->m_client_name = client_name;

	this->m_player_sync_data = new packet_player_sync_data();

	networking::modules::c_module_player_sync::instance()->m_players.at(m_network_id) = this;
}

void c_player_entity::spawn(sdk_vec3_t position)
{
	static auto networking = c_server_networking::instance();

	librg_entity_t* librg_entity = librg_entity_fetch(&networking->m_ctx, m_network_id);
	if (!librg_entity) {
		return;
	}

	librg_entity->position = position;
	m_position = position;

	librg_lambda_message_send_to(&networking->m_ctx, NETWORK_SPAWN_PLAYER, librg_entity->client_peer, [&](librg_data* data) {
		librg_data_wptr(data, &m_position, sizeof(sdk_vec3_t));
	});
}
