#include "hooking.hpp"

#include "hooks/hook_crashfix.hpp"
#include "hooks/hook_disable_npc.hpp"
#include "hooks/hook_game.hpp"

#define HOOK_INSTANCE(class_instance)	\
	if(!##class_instance::instance()->hook()) {	\
		c_log::Info("(c_hooking):", "Failed to start hook:", #class_instance); \
		return false;	\
	} else c_log::Info("(c_hooking):", "Started hooking instance:", #class_instance); \


bool c_hooking::initialize()
{
	HOOK_INSTANCE(c_hook_crashfix);
	HOOK_INSTANCE(c_hook_disable_npc);
	HOOK_INSTANCE(c_hook_game);

	return false;
}
