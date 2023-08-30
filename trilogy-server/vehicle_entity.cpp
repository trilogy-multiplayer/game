#include <networking/entities/vehicle_entity.hpp>

#include <networking/server_networking.hpp>
#include <networking/modules/module_vehicle-sync.hpp>
#include <utilities/ida.hpp>

#include <future>

c_vehicle_entity::c_vehicle_entity(int32_t network_id, int32_t model_index)
{
	this->m_network_id = network_id;

	this->m_entity_type = e_entity_types::VEHICLE;
	this->m_model_index = model_index;

	GET_TARGET_SYNC_MODULE->m_vehicles.at(m_network_id) = this;
}

void c_vehicle_entity::on_entity_create(librg_event* event) 
{ 

	std::async([&]() {

		});
	// TODO: pass primary & secondary colors to client
}

//TODO: delete
void c_vehicle_entity::on_entity_remove(librg_event* event) { }

void c_vehicle_entity::on_entity_update(librg_event* event)
{
	
}

void c_vehicle_entity::on_local_client_stream(librg_event* event)
{
	IS_VALID_READABLE_PACKET;


}
