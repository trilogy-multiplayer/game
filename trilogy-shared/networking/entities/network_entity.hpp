#pragma once

#include <cstdint>
#include <definitions.hpp>

#include <vendor/librg/librg.h>

#define IS_VALID_READABLE_PACKET \
	if (librg_data_capacity(event->data) <= 0) { \
		librg_event_reject(event); \
		return; \
	} \
	
class c_network_entity
{
public:
	e_entity_types m_entity_type;
	int32_t m_network_id;

public:
	virtual void on_local_client_stream(librg_event* event) { }
	
	virtual void on_entity_create(librg_event* event) { }
	virtual void on_entity_remove(librg_event* event) { }
	virtual void on_entity_update(librg_event* event) { }
};