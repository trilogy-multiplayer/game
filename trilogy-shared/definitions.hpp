#pragma once

enum e_build_channel : int8_t {
    DEVELOPMENT = 1,
    BETA,
    RELEASE_CANDIDATE,
    RELEASE
};

#define TRILOGY_VERSION_MAJOR 1
#define TRILOGY_VERSION_MINOR 0
#define TRILOGY_VERSION_PATCH 0
#define TRILOGY_BUILD_CHANNEL 1

#define		MAX_PLAYERS		256
#define		MAX_VEHICLES	256
#define		MAX_ENTITIES	(MAX_PLAYERS + MAX_VEHICLES)

#define		SPAWN_POS_X		2290.74f  
#define		SPAWN_POS_Y		-1527.06f
#define		SPAWN_POS_Z		26.875f

enum e_network_events : int8_t {
	NETWORK_PLAYER_CONNECT = 13,
	NETWORK_SPAWN_PLAYER
};

enum class e_entity_types {
	EMPTY,
	PLAYER,
	VEHICLE,
};

