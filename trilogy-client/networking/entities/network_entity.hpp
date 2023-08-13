#pragma once

#include <cstdint>
#include <definitions.hpp>
#include <memory/memory.hpp>
#include <sdk/sdk_ped.hpp>

#include <vendor/librg/librg.h>

class c_network_entity
{
public:
	e_entity_types entity_type;
	int32_t network_id;

public:
	virtual void on_client_stream(librg_event* event) { }
	
	void on_entity_create();
	void on_entity_remove();
	void on_entity_update();
};