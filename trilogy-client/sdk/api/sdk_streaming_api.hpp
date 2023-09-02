#pragma once

#ifndef TRILOGY_SDK_STREAMING_API_H
#define TRILOGY_SDK_STREAMING_API_H

#include <common.hpp>
#include <memory/memory.hpp>

namespace sdk::api {
	class sdk_streaming_api : public c_singleton<sdk_streaming_api> {
	public:
		void initialize();

		bool has_model_loaded(int32_t model_index);
		void load_model(int32_t model_index, int32_t streaming_flags = 22);
		void load_all_requested_models();
	private:
		std::list<int32_t> m_loaded_models{};

		using sdk_request_model_t = void(*)(int32_t model_index, int32_t streaming_flags, int8_t unk);
		sdk_request_model_t m_request_model;

		using sdk_load_all_requested_models_t = void(*)();
		sdk_load_all_requested_models_t m_load_all_requested_models;

	private:
		bool m_is_loading_model = false;
	};
}

#endif