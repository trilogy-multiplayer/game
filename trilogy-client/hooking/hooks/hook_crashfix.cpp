#include "hook_crashfix.hpp"

#include <vendor/minhook/minhook.hpp>

int64_t h_crash_fix() {
	auto instance = c_hook_crashfix::instance();

	return *instance->crash_fix_collision_ptr += 1;
}

void h_crash_fix_2(int64_t this_ptr, int64_t unk) {}

int16_t h_crash_fix_3() { 
	return 1;
}

bool c_hook_crashfix::hook()
{
	auto hook_result = true;

	if (hook_result) {
		auto crash_fix = memory::find_pattern<crash_fix_t>(memory::module_t(nullptr), "c_hook_crashfix::crash_fix", "0F B7 05 ? ? ? ? B9 ? ? ? ? 66 3B C1");
		this->crash_fix_collision_ptr = memory::as_relative<int64_t*>(crash_fix);

		MH_CreateHook(crash_fix, h_crash_fix, reinterpret_cast<void**>(&o_crash_fix));
		hook_result = MH_EnableHook(crash_fix) == MH_OK;
	}

	if (hook_result) {
		auto crash_fix_2 = memory::find_pattern<crash_fix_2_t>(memory::module_t(nullptr), "c_hook_crashfix::crash_fix_2", "4C 8B C1 33 C9 41 83 B8 ? 07 ? ? ?");
		MH_CreateHook(crash_fix_2, h_crash_fix_2, reinterpret_cast<void**>(&o_crash_fix_2));
		hook_result = MH_EnableHook(crash_fix_2) == MH_OK;
	}

	if (hook_result) {
		auto crash_fix_3 = memory::find_pattern<crash_fix_3_t>(memory::module_t(nullptr), "c_hook_crashfix::crash_fix_3", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 C8 4C 8D 25 ? ? ? ?");
		MH_CreateHook(crash_fix_3, h_crash_fix_3, reinterpret_cast<void**>(&o_crash_fix_3));
		hook_result = MH_EnableHook(crash_fix_3) == MH_OK;
	}

	return hook_result;
}
