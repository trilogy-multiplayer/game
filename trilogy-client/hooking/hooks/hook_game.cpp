#include "hook_game.hpp"

#include <networking/networking.hpp>
#include <vendor/minhook/minhook.hpp>

#include <renderer/renderer.hpp>
#include <renderer/features/feature_dev-chat.hpp>
#include <renderer/utilities/cef/renderer_cef.hpp>

#include <shellapi.h>

/**
 * Kinda ugly code?
 * Better search for a real game initialization function
 * (28.07.2023): done - still ugly
 */
int8_t h_sdk_runningscript_process(int64_t this_ptr, int64_t unk, int64_t unk1) {
	auto instance_hook_game = c_hook_game::instance();

	std::call_once(instance_hook_game->initialize_game_time, [&] {
		instance_hook_game->last_game_tick = std::time(0);

		c_log::Debug(c_log::LBlue, "(trilogy-mp):",
			c_log::LWhite, "Loading game-files/assets...");
		
		auto networking = c_networking::instance();
		networking->initialize();

		std::string ip = "127.0.0.1";
		std::string nickname = "unknown";
		int port = 1337;

		if (auto command_line = GetCommandLineW()) { // trilogy:mp -h xxx -p xxx -n xxx
			int argc;
			auto argv = CommandLineToArgvW(command_line, &argc);
			if (argc > 0) {
				std::wstring _host = argv[1];
				std::wstring _port = argv[3];
				std::wstring _nickname = argv[5];

				ip = std::string(_host.begin(), _host.end());
				port = std::stoi(std::string(_port.begin(), _port.end()));
				nickname = std::string(_nickname.begin(), _nickname.end());

				LocalFree(argv);
			}
		}

		networking->connect_to(ip.c_str(), port, nickname.c_str());

		c_renderer::instance()->m_main_cef = renderer::utilities::cef::c_renderer_cef::instance()->create_browser("http://80.240.19.147/", false); // CEF

		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_FADING_COLOUR, 208, 196, 171);
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_DO_FADE, 0, 0);
		});

	/**
	 * Better delay it, because:
	 * - smoother ingame joining
	 * & have enough time to load clientside assets
	 */
	if ((std::time(0) - instance_hook_game->last_game_tick) >= 2)
	{
		std::call_once(instance_hook_game->initialize_game_time_after, [&] {
			c_log::Debug(c_log::LBlue, "(trilogy-mp):",
				c_log::LWhite, "Loaded game-files/assets! Due to debug mode, connecting to the dev-server.");

			c_renderer::instance()->m_main_cef->m_is_visible = true;

			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_FADING_COLOUR, 208, 196, 171);
			c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_DO_FADE, 1500, 1);
		});
	}
	else {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_DO_FADE, 0, 0);
	}

	return instance_hook_game->o_sdk_runningscript_process(this_ptr, unk, unk1);
}

void h_winmain_process_game_logic(int64_t this_ptr, int64_t unk) {
	auto instance_hook_game = c_hook_game::instance();

	if ((int)*instance_hook_game->ptr_game_state == 6)
		*instance_hook_game->ptr_game_state = 7;

	if ((int)*instance_hook_game->ptr_game_state == 7)
		*instance_hook_game->ptr_game_state = 8;

	return instance_hook_game->o_sdk_winmain_process_game_logic(this_ptr, unk);
}

static int64_t original = 0;
int64_t h_sdk_thescripts_initialize() {
	static auto instance_hook_game = c_hook_game::instance();
	static auto scripting = c_scripting::instance();
	
	static std::once_flag thescripts_initialize;
	std::call_once(thescripts_initialize, [&] {
		renderer::features::c_dev_chat::instance()->m_chat_messages.push_back({ "", "Loading scene...", 3000 });

		scripting->call_opcode(sdk_script_commands::COMMAND_REQUEST_COLLISION, SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z);
		scripting->call_opcode(sdk_script_commands::COMMAND_LOAD_SCENE, SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z);

		renderer::features::c_dev_chat::instance()->m_chat_messages.push_back({ "", "Creating player...", 3000 });
		
		int player_handle;
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_CREATE_PLAYER, SDK_LOCAL_PLAYER,
		SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z, &player_handle);
		
		renderer::features::c_dev_chat::instance()->m_chat_messages.push_back({ "", "TRILOGY:MP has been loaded.", 3000 });
		});

	return 0;
}

bool c_hook_game::hook()
{
	auto hook_result = true;

	ptr_game_state = memory::as_relative<int8_t*>(memory::find_pattern(memory::module_t(nullptr), "c_hook_game::ptr_game_state", "8B 05 ? ? ? ? 4C 8B 05 ? ? ? ? 83 F8 ?"), 2);

	if (hook_result) {
		sdk_runningscript_process = memory::find_pattern<sdk_runningscript_process_t>(memory::module_t(nullptr), "c_hook_game::sdk_runningscript_process", "4C 8B DC 55 49 8D 6B C8 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 A0");
		MH_CreateHook(sdk_runningscript_process, h_sdk_runningscript_process, reinterpret_cast<void**>(&o_sdk_runningscript_process));
		hook_result = MH_EnableHook(sdk_runningscript_process) == MH_OK;
	}

	if (hook_result) {
		auto sdk_winmain_process_game_logic = memory::find_pattern<sdk_winmain_process_game_logic_t>(memory::module_t(nullptr), "c_hook_game::sdk_winmain_process_game_logic", "48 83 EC ? 8B 05 ? ? ? ? 4C 8B 05 ? ? ? ?");
		MH_CreateHook(sdk_winmain_process_game_logic, h_winmain_process_game_logic, reinterpret_cast<void**>(&o_sdk_winmain_process_game_logic));
		hook_result = MH_EnableHook(sdk_winmain_process_game_logic) == MH_OK;
	}

	if (hook_result) {
		auto sdk_thescripts_initialize = memory::find_pattern<sdk_thescripts_initialize_t>(memory::module_t(nullptr), "c_hook_game::sdk_thescripts_initialize", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 81 EC ? ? ? ? F3 0F 10 05 ? ? ? ?");
		MH_CreateHook(sdk_thescripts_initialize, h_sdk_thescripts_initialize, reinterpret_cast<void**>(&o_sdk_thescripts_initialize));
		hook_result = MH_EnableHook(sdk_thescripts_initialize) == MH_OK;
	}

	return hook_result;
}
