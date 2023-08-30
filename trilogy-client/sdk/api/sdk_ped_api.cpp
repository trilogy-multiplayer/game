#include "sdk_ped_api.hpp"

#include <sdk/api/sdk_streaming_api.hpp>

void sdk::api::sdk_ped_api::initialize()
{
	m_rotation_offset = *memory::find_pattern<int32_t*>(memory::module_t(nullptr), "sdk::api::sdk_ped_api::m_rotation_offset",
		"F3 0F 11 93 ? ? ? ? 41 0F 54 C5", 0x4);

	m_force_power_offset = *memory::find_pattern<int8_t*>(memory::module_t(nullptr), "sdk::api::sdk_ped_api::m_force_power_offset",
		"F3 0F 10 05 ? ? ? ? F3 0F 11 7F ? F3 0F 11", 0xC);

	m_set_model_index = memory::find_pattern<sdk_set_model_index_t>(memory::module_t(nullptr),
		"sdk::api::sdk_ped_api::m_set_model_index",
		"48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 81 49 30 ? ? ? ?");
}

sdk_vec2_t sdk::api::sdk_ped_api::get_rotation(sdk_ped* player)
{
	if (!m_rotation_offset) {
		return sdk_vec2_t();
	}

	int64_t player_ptr = (int64_t)player;

	return *(sdk_vec2_t*)(player_ptr + m_rotation_offset);
}

void sdk::api::sdk_ped_api::set_rotation(sdk_ped* player, sdk_vec2_t rotation)
{
	if (!m_rotation_offset) {
		return;
	}

	int64_t player_ptr = (int64_t)player;

	*(sdk_vec2_t*)(player_ptr + m_rotation_offset) = rotation;
}

sdk_vec2_t sdk::api::sdk_ped_api::get_force_power(sdk_ped* player)
{
	if (!m_force_power_offset) {
		return sdk_vec2_t();
	}

	int64_t player_ptr = (int64_t)player;

	return *(sdk_vec2_t*)(player_ptr + m_force_power_offset);
}

void sdk::api::sdk_ped_api::set_force_power(sdk_ped* player, sdk_vec2_t force_power)
{
	if (!m_force_power_offset) {
		return;
	}

	int64_t player_ptr = (int64_t)player;

	*(sdk_vec2_t*)(player_ptr + m_force_power_offset) = force_power;
}

void sdk::api::sdk_ped_api::set_model(sdk_ped* player, int32_t model_index)
{
	static auto streaming_api = sdk::api::sdk_streaming_api::instance();
	
	streaming_api->load_model(model_index);

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_PLAYER_MODEL, 0, model_index);

	//return m_set_model_index(player, model_index);
}
