#include "feature_benchmark.hpp"

/**
  * Maybe switch benchmarking to GetTickCount
  * Is std::time(0) accurate?
  */
#define RUN_BENCHMARK(function)	\
	auto start_time_##function = std::time(0);	\
	for(int i = 0; i <= 99999; i++) function();	\
	auto time_difference_##function = std::time(0) - start_time_##function;	\
	c_log::Info(c_log::LGreen, "(c_benchmark::run_benchmark):",	\
		c_log::LCyan, std::string(#function), "took", c_log::Join(time_difference_##function, "ms"));	\

void run_health_benchmark() {
	static int char_id = 0;
	if (char_id == 0) {
		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_PLAYER_CHAR, 0, &char_id);
	}

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_SET_CHAR_HEALTH, char_id, 55);
}

void run_health_opcode_benchmark() {
	static sdk_ped* player = nullptr;
	if (player == nullptr) {
		player = c_memory::instance()->sdk_find_player_ped(0);
	}

	player->m_vec_speed = sdk_vec3_t(0, 0, 0);
}

void networking::features::c_feature_benchmark::run_benchmark()
{
	RUN_BENCHMARK(run_health_benchmark);
	RUN_BENCHMARK(run_health_opcode_benchmark);
}
