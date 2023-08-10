#pragma once

#include "utilities/logging.hpp"
#include "utilities/singleton.hpp"

#include <memory/module.hpp>
#include <memory/patterns.hpp>

class c_hook_crashfix : public c_singleton<c_hook_crashfix> {
private:
	/**
     * Need some more reversal.
	 * crash_fix is some kind of collision function - not present in the original gta san andreas
	 * responsible for crashes with existing network players (create player -> crash)
	 * 
	 * crash_fix_2 kinda too lol. 
	 * (not important <- just nop/hook it )
	 */

	using crash_fix_t = int64_t(*)();
	crash_fix_t o_crash_fix = 0;

	using crash_fix_2_t = void(*)(int64_t this_ptr, int64_t unk);
	crash_fix_2_t o_crash_fix_2 = 0;

public:
	int64_t* crash_fix_collision_ptr = 0;
	
	bool hook();
};