#pragma once

#include "utilities/logging.hpp"
#include "utilities/singleton.hpp"

#include <memory/module.hpp>
#include <memory/patterns.hpp>

class c_hook_disable_npc : public c_singleton<c_hook_disable_npc> {
private:
	using sdk_population__update_t = int8_t(*)();
	using sdk_carctrl__update_t = int8_t(*)(int64_t this_ptr);

	sdk_population__update_t	sdk_population__update;
	sdk_carctrl__update_t		sdk_carctrl__update;
public:
	bool hook();
};