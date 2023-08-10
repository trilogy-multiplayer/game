#include "memory.hpp"

#include <vendor/minhook/minhook.hpp>
#include <utilities/ida.hpp>

#include <mutex>
#include <hooking/hooking.hpp>
/*
#define CREATE_HOOK(ptr)		\
	int8_t h_##ptr() {				\
		return 0;					\
	}								\

#define ENABLE_HOOK(ptr, type)										  \
	static type o_##ptr = 0;										  \
	MH_CreateHook(ptr, h_##ptr , reinterpret_cast<void**>(&o_##ptr)); \
	MH_EnableHook(ptr);											      \

CREATE_HOOK(sdk_population__update);
CREATE_HOOK(sdk_carctrl__update);
*/

// TODO CLEANUP

template<class A, class B = A>
static void increase_pool_size(CPool<A, B>* pool, int nSize)
{
	pool->m_pObjects = static_cast<B*>(operator new(sizeof(B) * nSize));
	pool->m_byteMap = static_cast<tPoolObjectFlags*>(operator new(nSize));

	pool->m_nSize = nSize;
	pool->m_nFirstFree = -1;

	for (int i = 0; i < nSize; ++i)
	{
		pool->m_byteMap[i].bEmpty = true;
		pool->m_byteMap[i].nId = 0;
	}
}

int64_t h_sdk_find_player_ped(int32_t player_id) {
	return c_memory::instance()->o_sdk_find_player_ped(player_id);
}

void c_memory::initialize()
{
	MH_Initialize();

	c_scripting::instance()->initialize();

	/**
	 * TODO:
	 * Split this into hooking / patches classes
	 */
	auto base_module = memory::module_t(nullptr);

	sdk_find_player_ped = memory::find_pattern<sdk_find_player_ped_t>(base_module, "c_memory::sdk_find_player_ped", "85 C9 79 ? 0F B6 0D ? ? ? ? 48 63 C1 48 8D 0D ? ? ? ?");
	MH_CreateHook(sdk_find_player_ped, h_sdk_find_player_ped, reinterpret_cast<void**>(&o_sdk_find_player_ped));
	MH_EnableHook(sdk_find_player_ped);
	// increase_pool_size()

	c_hooking::instance()->initialize();

	sdk_player_in_focus = memory::as_relative<int8_t*>(memory::find_pattern(base_module, "player_in_focus", "44 88 2D ? ? ? ? 66 0F 1F 44 00 00"));
	sdk_player_pad = memory::as_relative<CPad*>(memory::find_pattern(base_module, "player_pad", "48 8D 05 ? ? ? ? F3 0F 5C 05 ? ? ? ?"));

	auto pad = memory::as_relative<int64_t*>(memory::find_pattern(base_module, "player_pad2", "48 8B 14 C3 48 85 D2 74 ? 48 8B 02 48 8B CA FF 50 30 48 8B 0D ? ? ? ? 85 C0 74 ? 83 79 10 ? 75 ? 48 8B 05 ? ? ? ? 48 85 C0 74 ? 80 78 68 ? 0F 85 ? ? ? ?"));

	time_step = memory::as_relative<float*>(memory::find_pattern(memory::module_t(nullptr), "get_time_step", "8B 05 ? ? ? ? 48 8D 54 24 30 0F C8 89 44 24 30 48 8B 07 41 B8 ? ? ? ? 48 8B CF FF 90 50 01 00 00 48 8B 4F 08 48 8B 11 48 8D 42 04 48 3B 41 08 77 ? 8B 02 89 05 ? ? ? ? 48 8B 47 08 48 83 00 ? EB ? F6 47 29 ? 75 ? 48 8D 15 ? ? ? ? EB ? F6 47 28 ? 74 ? 48 8B 07 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? 48 8B CF FF 90 50 01 00 00 8B 05 ? ? ? ? 0F C8 89 05 ? ? ? ? EB ? 8B 05 ? ? ? ? 48 8D 54 24 30 0F C8 89 44 24 30 48 8B 07 41 B8 ? ? ? ? 48 8B CF FF 90 50 01 00 00 48 8B 4F 08 48 8B 11 48 8D 42 04 48 3B 41 08 77 ? 8B 02 89 05 ? ? ? ? 48 8B 47 08 48 83 00 ? EB ? F6 47 29 ? 75 ? 48 8D 15 ? ? ? ? EB ? F6 47 28 ? 74 ? 48 8B 07 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? 48 8B CF FF 90 50 01 00 00 8B 05 ? ? ? ? 0F C8 89 05 ? ? ? ? EB ? 8B 05 ? ? ? ? 48 8D 54 24 30 0F C8 89 44 24 30 48 8B 07 41 B8 ? ? ? ? 48 8B CF FF 90 50 01 00 00 48 8B 4F 08 48 8B 11 48 8D 42 02"), 2);

	sdk_ped_pool_padding = memory::find_pattern<int32_t*>(base_module, "sdk_ped_pool_padding", "48 69 C0 ? ? ? ? 49 03 00 48 8B 5C 24 08 C3 48 8B C3 48 8B 5C 24 08 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ?", 3);
	sdk_ped_pool = memory::as_relative<int64_t*>(memory::find_pattern(base_module, "pool", "48 8B 0D ? ? ? ? 44 8B 0D ? ? ? ?"));
}
