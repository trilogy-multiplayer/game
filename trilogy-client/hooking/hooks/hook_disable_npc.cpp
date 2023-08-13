#include "hook_disable_npc.hpp"

#include <vendor/minhook/minhook.hpp>

char h_sdk_population__update() {
	return 0;
}

char h_sdk_carctrl__update() {
	return 1;
}

bool c_hook_disable_npc::hook()
{
	auto base_module = memory::module_t(nullptr);
	auto hook_result = true;

	if (hook_result) {
		// Disable NPC-Ped Traffic
		sdk_population__update = memory::find_pattern<sdk_population__update_t>(base_module, "c_hook_disable_npc::sdk_population__update", "40 53 48 83 EC ? 0F BF ? ? ? ? ? 33 DB");
		MH_CreateHook(sdk_population__update, h_sdk_population__update, nullptr);
		hook_result = MH_EnableHook(sdk_population__update) == MH_OK;

	}

	if (hook_result) {
		// Disable NPC-Vehicle Traffic
		sdk_carctrl__update = memory::find_pattern<sdk_carctrl__update_t>(base_module, "c_hook_disable_npc::sdk_carctrl__update", "48 83 EC ? 80 79 3C ? 74 ? B0 ?");
		MH_CreateHook(sdk_carctrl__update, h_sdk_carctrl__update, nullptr);
		hook_result = MH_EnableHook(sdk_carctrl__update) == MH_OK;
	}

	return hook_result;
}
