#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_NETWORKING_FEATURE_NAMETAGS_H
#define TRILOGY_NETWORKING_FEATURE_NAMETAGS_H

#include <common.hpp>
#include <renderer/utilities/imgui_render.hpp>
#include <networking/networking.hpp>

// ehm, wrong category? maybe better move to renderer

namespace networking::features {
	class c_nametags : public c_singleton<c_nametags> {
	public:
		void on_tick();
	};
}
#endif