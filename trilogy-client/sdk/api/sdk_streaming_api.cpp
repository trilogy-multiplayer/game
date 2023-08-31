#include "sdk_streaming_api.hpp"

void sdk::api::sdk_streaming_api::initialize()
{
	m_request_model = memory::find_pattern<sdk_request_model_t>(memory::module_t(nullptr),
		"sdk::api::sdk_streaming_api::m_request_model",
		"40 55 53 56 41 55 41 56 41 57 48 8D AC 24 78 F8 FF FF");

	m_load_all_requested_models = memory::find_pattern<sdk_load_all_requested_models_t>(memory::module_t(nullptr),
		"sdk::api::sdk_streaming_api::m_load_all_requested_models",
		"48 83 EC ? 48 8B 0D ? ? ? ? 48 8B 01 FF 90 58 01 00 00 48 8B C8");
}

bool sdk::api::sdk_streaming_api::has_model_loaded(int32_t model_index)
{
	/**
	  * Afaik, theres no function for it in SA:DE - so just call the opcode.
	  * Maybe ill find it lol.
	  */
	
	/* int has_model_loaded;
	 * c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_HAS_MODEL_LOADED, model_index, &has_model_loaded);

	 * return (bool)has_model_loaded;
	 */

	auto it = std::find(m_loaded_models.begin(), m_loaded_models.end(), model_index);

	if (it != m_loaded_models.end()) return true;
	else return false;
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

	// if (has_model_loaded(model_index)) return;

	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_REQUEST_MODEL, model_index);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	/**
	  * TODO:
	  * Find the perfect timeouts to load the models.
	  */
	c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_LOAD_ALL_MODELS_NOW);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	c_log::Info("Loaded");
}

void sdk::api::sdk_streaming_api::load_all_requested_models()
{
	return m_load_all_requested_models();
}

