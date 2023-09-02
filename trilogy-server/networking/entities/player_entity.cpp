#include <networking/entities/player_entity.hpp>

#include <networking/server_networking.hpp>
#include <networking/modules/module_player-sync.hpp>
#include <utilities/ida.hpp>

c_player_entity::c_player_entity(int32_t network_id, std::string client_name)
{
	this->m_network_id = network_id;

	this->m_entity_type = e_entity_types::PLAYER;
	this->m_name = client_name;

	GET_TARGET_SYNC_MODULE->m_players.at(m_network_id) = this;
}

/**
  * TODO: implement these 
  */
void c_player_entity::on_entity_create(librg_event* event) 
{
	librg_data_wu32(event->data, m_model_index);
}

void c_player_entity::on_entity_remove(librg_event* event) { }

void c_player_entity::on_entity_update(librg_event* event)
{
	librg_data_wptr(event->data, &m_position, sizeof(sdk_vec3_t));
	librg_data_wptr(event->data, &m_vec_speed, sizeof(sdk_vec3_t));

	librg_data_wf32(event->data, m_heading);

	librg_data_wptr(event->data, &m_rotation, sizeof(sdk_vec2_t));
	librg_data_wptr(event->data, &m_force_power, sizeof(sdk_vec2_t));
	librg_data_wptr(event->data, &m_camera_front, sizeof(sdk_vec3_t));

	librg_data_wu32(event->data, m_health);
	librg_data_wu32(event->data, m_armor);

	librg_data_wptr(event->data, &m_hid_mapping, sizeof(hid::hid_compressed_mapping));
}

void c_player_entity::on_local_client_stream(librg_event* event)
{
	IS_VALID_READABLE_PACKET;

	librg_data_rptr(event->data, &m_position, sizeof(sdk_vec3_t));
	librg_data_rptr(event->data, &m_vec_speed, sizeof(sdk_vec3_t));

	m_heading = librg_data_rf32(event->data);

	librg_data_rptr(event->data, &m_rotation, sizeof(sdk_vec2_t));
	librg_data_rptr(event->data, &m_force_power, sizeof(sdk_vec2_t));
	librg_data_rptr(event->data, &m_camera_front, sizeof(sdk_vec3_t));

	m_health = librg_data_ru32(event->data);
	m_armor = librg_data_ru32(event->data);

	librg_data_rptr(event->data, &m_hid_mapping, sizeof(hid::hid_compressed_mapping));
}

void c_player_entity::spawn(sdk_vec3_t position, int32_t model_index)
{
	static auto networking = c_server_networking::instance();

	librg_entity_t* librg_entity = librg_entity_fetch(&networking->m_ctx, m_network_id);
	if (!librg_entity) {
		return;
	}

	librg_entity->position = position;
	m_position = position;

	m_model_index = model_index;

	librg_send_instream(&networking->m_ctx, NETWORK_SPAWN_PLAYER, librg_entity->id, librg_lambda(data), {
		librg_data_wu32(&data, m_network_id);
		librg_data_wptr(&data, &m_position, sizeof(sdk_vec3_t));
		librg_data_wu32(&data, m_model_index);
	});
}
