#pragma once

#define		MAX_PLAYERS		256
#define		MAX_VEHICLES	256
#define		MAX_ENTITIES	(MAX_PLAYERS + MAX_VEHICLES)

#define		SPAWN_POS_X		2290.74f  
#define		SPAWN_POS_Y		-1527.06f
#define		SPAWN_POS_Z		26.875f

enum e_network_events : int8_t {
	SPAWN_PLAYER
};

enum class e_entity_types {
	EMPTY,
	PLAYER,
	VEHICLE,
};

#define TRILOGY_VERSION "0.0.1"