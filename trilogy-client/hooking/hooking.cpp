#include "hooking.hpp"

#include "hooks/hook_crashfix.hpp"
#include "hooks/hook_disable_npc.hpp"
#include "hooks/hook_game.hpp"

#define HOOK_INSTANCE(class_instance)	\
	if(!##class_instance::instance()->hook()) {	\
		c_log::Info(c_log::LRed, "(c_hooking::initialize):", \
			c_log::LWhite, "Failed to start hook:",	\
			c_log::LCyan, #class_instance);	\
		return false;	\
	} else { \
		c_log::Info(c_log::LGreen, "(c_hooking::initialize):",	\
		c_log::LWhite, "Started hooking instance:",	\
		c_log::LCyan, #class_instance);	\
	} \


bool c_hooking::initialize()
{
	HOOK_INSTANCE(c_hook_crashfix);
	HOOK_INSTANCE(c_hook_disable_npc);
	HOOK_INSTANCE(c_hook_game);

	return false;
}
