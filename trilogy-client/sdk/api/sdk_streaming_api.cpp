#include "sdk_streaming_api.hpp"

void sdk::api::sdk_streaming_api::initialize()
{
	m_request_model = memory::find_pattern<sdk_request_model_t>(memory::module_t(nullptr),
		"sdk::api::sdk_streaming_api::m_request_model",
		"40 55 53 56 41 55 41 56 41 57 48 8D AC 24 78 F8 FF FF");

	m_load_all_requested_models = memory::find_pattern<sdk_load_all_requested_models_t>(memory::module_t(nullptr),
		"sdk::api::sdk_streaming_api::m_load_all_requested_models",
		"48 83 EC ? 48 8B 0D ? ? ? ? 48 8B 01 FF 90 58 01 00 00 48 8B C8");

	c_log::Info(c_log::Red, "Pattern:", memory::find_pattern(memory::module_t(nullptr), "world_players", "4C 8D 05 ? ? ? ? 48 03 C0 41 80 3C C0 ?"));
	m_streaming_model_load_state = *(int64_t* (*)[])memory::as_relative(memory::find_pattern(memory::module_t(nullptr), "world_players", "4C 8D 05 ? ? ? ? 48 03 C0 41 80 3C C0 ?"));

	c_log::Info(c_log::Red, "Relative pattern:", m_streaming_model_load_state);
}

bool sdk::api::sdk_streaming_api::has_model_loaded(int32_t model_index)
{
	/**
	  * Afaik, theres no function for it in SA:DE - so just call the opcode.
	  * Maybe ill find it lol.
	  */


	// c_log::Info(m_streaming_model_load_state[18 * 7]);
	// c_log::Info([18 * 1]);

	return c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_HAS_MODEL_LOADED, model_index);

	// auto it = std::find(m_loaded_models.begin(), m_loaded_models.end(), model_index);

	// if (it != m_loaded_models.end()) return true;
	// else return false;
}

void sdk::api::sdk_streaming_api::load_model(int32_t model_index, int32_t streaming_flags)
{
	/**
	  * Code looks bastard,
	  * https://github.com/gtco-op/vc-coop/blob/master/vice-city-coop/vccoop_main/CGame.cpp#L198
	  */

	  /*while (!has_model_loaded(model_index)) {
		  if (!m_is_loading_model) {
			  m_request_model(model_index, streaming_flags, 0);
			  std::this_thread::sleep_for(std::chrono::milliseconds(250));
			  m_load_all_requested_models();
			  m_is_loading_model = true;
		  }
		  std::this_thread::sleep_for(std::chrono::milliseconds(1));
	  }

	  m_is_loading_model = false;*/

	c_log::Info("1 Model loaded opcode state:", has_model_loaded(model_index));

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_REQUEST_MODEL, model_index);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	/**
	  * TODO:
	  * Find the perfect timeouts to load the models.
	  */
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_LOAD_ALL_MODELS_NOW);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	c_log::Info("2 Model loaded opcode state:", has_model_loaded(model_index));
}

void sdk::api::sdk_streaming_api::load_all_requested_models()
{
	return m_load_all_requested_models();
}

