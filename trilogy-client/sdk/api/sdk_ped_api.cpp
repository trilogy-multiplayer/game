#include "sdk_ped_api.hpp"

void sdk::api::sdk_ped_api::initialize()
{
	m_rotation_offset = *memory::find_pattern<int32_t*>(memory::module_t(nullptr), "sdk::api::sdk_ped_api::m_rotation_offset",
		"F3 0F 11 93 ? ? ? ? 41 0F 54 C5", 0x4);

	m_force_power_offset = *memory::find_pattern<int8_t*>(memory::module_t(nullptr), "sdk::api::sdk_ped_api::m_force_power_offset",
		"F3 0F 10 05 ? ? ? ? F3 0F 11 7F ? F3 0F 11", 0xC);
}

sdk_vec2_t sdk::api::sdk_ped_api::get_rotation(sdk_ped* player)
{
	if (!m_rotation_offset) {
		return sdk_vec2_t();
	}

	int64_t player_ptr = (int64_t)player;

	return sdk_vec2_t(
		floor(*(float*)(player_ptr + m_rotation_offset)),
		floor(*(float*)(player_ptr + (m_rotation_offset + 0x4)))
	);
}

void sdk::api::sdk_ped_api::set_rotation(sdk_ped* player, sdk_vec2_t rotation)
{
	if (!m_rotation_offset) {
		return;
	}

	int64_t player_ptr = (int64_t)player;

	*(float*)(player_ptr + m_rotation_offset) = rotation.x;
	*(float*)(player_ptr + (m_rotation_offset + 0x4)) = rotation.y;
}

sdk_vec2_t sdk::api::sdk_ped_api::get_force_power(sdk_ped* player)
{
	if (!m_force_power_offset) {
		return sdk_vec2_t();
	}

	int64_t player_ptr = (int64_t)player;

	return sdk_vec2_t(
		floor(*(float*)(player_ptr + m_force_power_offset)),
		floor(*(float*)(player_ptr + (m_force_power_offset + 0x4)))
	);
}

void sdk::api::sdk_ped_api::set_force_power(sdk_ped* player, sdk_vec2_t force_power)
{
	if (!m_force_power_offset) {
		return;
	}

	int64_t player_ptr = (int64_t)player;

	*(float*)(player_ptr + m_force_power_offset) = force_power.x;
	*(float*)(player_ptr + (m_force_power_offset + 0x4)) = force_power.y;
}
