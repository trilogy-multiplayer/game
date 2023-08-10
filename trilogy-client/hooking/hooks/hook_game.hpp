#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_HOOK_GAME_H
#define TRILOGY_HOOK_GAME_H

#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "utilities/logging.hpp"
#include "utilities/singleton.hpp"

#include <memory/module.hpp>
#include <memory/patterns.hpp>
#include <ctime>
#include <mutex>

class c_hook_game : public c_singleton<c_hook_game> {
private:
	using sdk_runningscript_process_t = int8_t(*)(int64_t this_ptr, int64_t unk, int64_t unk1);
	using sdk_winmain_process_game_logic_t = void(*)(int64_t this_ptr, int64_t unk);
	using sdk_thescripts_initialize_t = int64_t(*)();
	using sdk_thescripts_initialize2_t = void(*)(int64_t*);

	sdk_runningscript_process_t sdk_runningscript_process;
public:
	std::time_t last_game_tick;
	std::once_flag initialize_game_time;
	std::once_flag initialize_game_time_after;

	sdk_runningscript_process_t o_sdk_runningscript_process;
	sdk_winmain_process_game_logic_t o_sdk_winmain_process_game_logic;
	sdk_thescripts_initialize_t o_sdk_thescripts_initialize;

	int8_t* ptr_game_state;

	bool hook();
};

#endif