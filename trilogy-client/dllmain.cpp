#include "common.hpp"

#include "renderer/renderer.hpp"
#include "memory/memory.hpp"
#include "core/core.hpp"

#include <vendor/minhook/minhook.hpp>
#include <utilities/ida.hpp>
#include <definitions.hpp>
#include <sdk/sdk.hpp>
#include <networking/features/feature_benchmark.hpp>

#include <sdk/api/sdk_ped_api.hpp>

#include <delayimp.h>
#pragma comment(lib, "delayimp")

uintptr_t init_main(const HMODULE h_module)
{	
	auto core = c_core::instance();
	
	c_log::add_out(new c_log::c_log_consolestream);
	c_log::Info("Launching", TRILOGY_FULLNAME,
		c_log::Join(TRILOGY_VERSION_MAJOR, ".", TRILOGY_VERSION_MINOR, ".", TRILOGY_VERSION_PATCH),
		"on branch", TRILOGY_BuildChannelToString(TRILOGY_BUILD_CHANNEL));

	auto memory = c_memory::instance();
	memory->initialize();

	auto renderer = c_renderer::instance();
	renderer->initialize();

	int player_id = 2;

	sdk_vec3_t position_to_look_at{};

	while (true) {
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) break;
		
		if (GetAsyncKeyState(VK_F2) & 0x1) {
			//int char_id;
			//c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &char_id);
			//c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GIVE_WEAPON_TO_CHAR, char_id, 24, 100);
			networking::features::c_feature_benchmark::instance()->run_benchmark();

			// sdk_ped* player = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(0);
			// position_to_look_at = player->m_matrix->m_position;
			// c_renderer::instance()->focus_browser = !c_renderer::instance()->focus_browser;
		}

		if (GetAsyncKeyState(VK_BACK) & 0x1) {
			//sdk_player_ped* ped = (sdk_player_ped*)c_memory::instance()->sdk_find_player_ped(0);
			//ped->m_matrix->set_position(sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z));
			//c_log::Info(ped, ped->m_matrix);

			//pad_state->m_states[68].m_cur_state = 3;

			//c_log::Info("Ped pool", c_memory::instance()->sdk_ped_pool);
			//c_log::Info("Ped pool", c_memory::instance()->sdk_ped_pool->GetAt(0));

			/*
			for (int i = 0; i < 102; i++) {
				if (c_memory::instance()->sdk_hid_mapping->m_keyboard_states[i].m_cur_state == hid::e_hid_mapping_current_state::PRESSED)
					c_log::Info("Current pressed button:", i);
			}

			for (int i = 0; i < 3; i++) {
				if (c_memory::instance()->sdk_hid_mapping->m_mouse_states[i].m_cur_state == hid::e_hid_mapping_current_state::PRESSED)
					c_log::Info("Current pressed mouse:", i);
			}
			*/

			// sdk_ped* ped = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(0);
			// F3 0F 10 84 31 98 6B EB 04 0F 57 05 ? ? ? ?
			// F3 0F 10 8C 31 9C 6B EB 04
			// static auto dword_144EB6B98 = memory::find_pattern(memory::module_t(), "dword_144EB6B98", "48 8D 0D ? ? ? ? F3 0F 10 54 0A 04");
			// static auto dword_144EB6B9C = memory::find_pattern(memory::module_t(), "dword_144EB6B9C", "F3 0F 10 8C 31 9C 6B EB 04");

			// dword_144EB6B9C[110 * (unsigned __int8)TheCamera_ActiveCamera]

			//c_log::Info(ped);/*
			//ped->m_matrix->set_position(sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z));

			/*sdk_vec3_t screen_coords; float w, h;
			c_memory::instance()->sdk_calc_screen_coords(
				sdk_vec3_t(ped->m_matrix->pos_x, ped->m_matrix->pos_y, ped->m_matrix->pos_z + 1.25f),
				&screen_coords,
				&w,
				&h,
				true, true
			);

			c_log::Info("sdk_calc_screen_coords", screen_coords.x, screen_coords.y, screen_coords.z, w, h);*/

		    //	c_scripting::instance()->call_opcode(sdk_script_commands::task_look, char_id, position_to_look_at.x, position_to_look_at.y, position_to_look_at.z, -1);
		}

		if (GetAsyncKeyState(VK_ADD) & 0x1) {
			int char_id;
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &char_id);
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CURRENT_CHAR_WEAPON, char_id, 1.313131f);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	fclose(stdout);
	FreeConsole();

	FreeLibraryAndExitThread(h_module, 0);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE h_module, uintptr_t dw_reason_for_call, LPVOID lp_reserved)
{
	DisableThreadLibraryCalls(h_module);

	if (dw_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (AllocConsole())
		{
			HWND console_hwnd = GetConsoleWindow();
			RECT console_bound = { 900, 420 };
			RECT window_rect;

			SetConsoleTitleA("TRILOGY:MP | Debug");

			GetWindowRect(console_hwnd, &window_rect);
			MoveWindow(console_hwnd, window_rect.left, window_rect.top, console_bound.left, console_bound.top, true);

			SetWindowLong(console_hwnd, GWL_STYLE, GetWindowLong(console_hwnd, GWL_STYLE) | WS_BORDER);
			SetWindowLong(console_hwnd, GWL_EXSTYLE, GetWindowLong(console_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

			SetLayeredWindowAttributes(console_hwnd, 0, 230, 2);

			freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		}

		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(init_main), h_module, NULL, NULL);

		return true;
	}

	return false;
}
