#include "common.hpp"

#include "memory/memory.hpp"
#include "core/core.hpp"

#include <vendor/minhook/minhook.hpp>
#include <utilities/ida.hpp>
#include <definitions.hpp>
#include <sdk/sdk.hpp>

uintptr_t init_main(const HMODULE h_module)
{
	auto core = c_core::instance();

	c_log::add_out(new c_log::c_log_consolestream);
	c_log::Info(core->m_name, core->m_version);

	auto memory = c_memory::instance();
	memory->initialize();

	int player_id = 2;
	int player_handle;

	while (true) {
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) break;
		//}

		if (GetAsyncKeyState(VK_BACK) & 0x8000) {
			//sdk_player_ped* ped = (sdk_player_ped*)c_memory::instance()->sdk_find_player_ped(0);
			//ped->m_matrix->set_position(sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z));
			//c_log::Info(ped, ped->m_matrix);

			//pad_state->m_states[68].m_cur_state = 3;

			//c_log::Info("Ped pool", c_memory::instance()->sdk_ped_pool);
			//c_log::Info("Ped pool", c_memory::instance()->sdk_ped_pool->GetAt(0));

			for (int i = 0; i < 102; i++) {
				if (c_memory::instance()->sdk_hid_mapping->m_states[i].m_cur_state == e_hid_mapping_current_state::PRESSED)
					c_log::Info("Current pressed button:", i);
			}
		}

		if (GetAsyncKeyState(VK_ADD) & 0x1) {
			int game_id;
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &game_id);

			float x, y, z;
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_CHAR_COORDINATES, game_id,
				&x, &y, &z);

			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_CREATE_PLAYER, player_id, x, y, z, &player_handle);

			std::stringstream debug_message_stream;
			debug_message_stream << "(trilogy:debug) ";
			debug_message_stream << "Adding player with handle: ";
			debug_message_stream << player_handle;
			debug_message_stream << ".";
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_PRINT_HELP, debug_message_stream.str().c_str());

			//auto ped_ptr = *(_QWORD*)*c_memory::instance()->sdk_ped_pool + 2712 * ((__int64)player_handle >> 8);
			//auto ped = (sdk_ped*)ped_ptr;

			//ped->update_position(sdk_vec3_t(x + 3.0f + (player_id * 0.2f), y, z), *c_memory::instance()->time_step);

			player_id = player_id + 1;
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
	if (dw_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (AllocConsole())
		{
			HWND console_hwnd = GetConsoleWindow();
			RECT console_bound = { 900, 420 };
			RECT window_rect;

			SetConsoleTitleA("trilogy:MP | Debug");

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
