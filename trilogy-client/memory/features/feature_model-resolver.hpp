#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_MEMORY_FEATURE_MODELRESOLVER_H
#define TRILOGY_MEMORY_FEATURE_MODELRESOLVER_H

#include <common.hpp>
#include <functional>

#include <sdk/api/sdk_streaming_api.hpp>
#include <queue>

namespace memory::features {
	class c_model_resolver : public c_singleton<c_model_resolver> {
	private:
		struct c_model_resolver_worker_item {
			int32_t model_index;
			std::function<void(int32_t)> worker_callback;
		};

		std::queue<c_model_resolver_worker_item> m_worker_items;
		std::thread m_model_resolve_thread;
		std::mutex m_worker_mutex;
	public:
		void initialize();

		/**
		  * Be careful here,
		  * passed pointers have to be validated before callback
		  */
		void add_model_to_worker(int32_t model_index, std::function<void(int32_t)> worker_callback);
		void on_model_resolve_queue();
	};
}
#endif