#pragma once

#ifndef TRILOGY_SDK_PED_API_H
#define TRILOGY_SDK_PED_API_H

#include <common.hpp>
#include <memory/memory.hpp>

namespace sdk::api {
	class sdk_ped_api : public c_singleton<sdk_ped_api> {
	public:
		void initialize();

		sdk_vec2_t get_rotation(sdk_ped* player);
		void set_rotation(sdk_ped* player, sdk_vec2_t rotation);

		sdk_vec2_t get_force_power(sdk_ped* player);
		void set_force_power(sdk_ped* player, sdk_vec2_t force_power);

	private:
		int32_t m_rotation_offset;
		int32_t m_force_power_offset;
	};
}

#endif