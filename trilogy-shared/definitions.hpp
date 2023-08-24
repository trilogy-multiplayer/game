#pragma once

#ifndef TRILOGY_DEFINITIONS_H
#define TRILOGY_DEFINITIONS_H

enum e_build_channel : int8_t {
	DEVELOPMENT = 1,
	BETA,
	RELEASE_CANDIDATE,
	RELEASE
};

inline const char* TRILOGY_BuildChannelToString(int8_t status)
{
#define TRILOGY_ST2STR(x)    \
    case x:             \
        return #x;

	switch (status) {
		TRILOGY_ST2STR(DEVELOPMENT)
			TRILOGY_ST2STR(BETA)
			TRILOGY_ST2STR(RELEASE_CANDIDATE)
			TRILOGY_ST2STR(RELEASE)
	}

#undef TRILOGY_ST2STR

	return "(unknown)";
}

#define TRILOGY_FULLNAME "TRILOGY:MP"

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
	NETWORK_ACCEPT_CONNECTION = 13,
	NETWORK_PLAYER_CONNECT,
	NETWORK_SPAWN_PLAYER
};

enum class e_entity_types {
	EMPTY,
	PLAYER,
	VEHICLE,
};

#endif