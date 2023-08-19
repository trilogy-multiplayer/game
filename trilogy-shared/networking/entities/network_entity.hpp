#pragma once

#include <cstdint>
#include <definitions.hpp>

#include <vendor/librg/librg.h>

class c_network_entity
{
public:
	e_entity_types m_entity_type;
	int32_t m_network_id;

public:
	virtual void on_client_stream(librg_event* event) { }
	
	virtual void on_entity_create(librg_event* event) { }
	virtual void on_entity_remove(librg_event* event) { }
	virtual void on_entity_update(librg_event* event) { }
};