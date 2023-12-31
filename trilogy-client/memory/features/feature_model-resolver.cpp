#include "feature_model-resolver.hpp"

void memory::features::c_model_resolver::initialize()
{
	m_model_resolve_thread = std::thread([this] { this->on_model_resolve_queue(); });
	m_model_resolve_thread.detach();
}

void memory::features::c_model_resolver::add_model_to_worker(int32_t model_index, std::function<void(int32_t)> worker_callback)
{
	std::lock_guard<std::mutex> lock(m_worker_mutex);
	m_worker_items.push({ model_index, worker_callback });
}

void memory::features::c_model_resolver::on_model_resolve_queue()
{
	while (true)
	{
		c_model_resolver_worker_item item;

		{
			std::lock_guard<std::mutex> lock(m_worker_mutex);
			if (!m_worker_items.empty()) {
				item = m_worker_items.front();
				m_worker_items.pop();
			}
		}

		if (item.worker_callback) {	
			// There is something really fucked up with loading models
			// We need a workaround for loading models asap!
			// sdk::api::sdk_streaming_api::instance()->load_model(item.model_index);

			c_log::Debug(c_log::Green, "(memory::features::c_model_resolver::on_model_resolve_thread):",
				c_log::LWhite, "Calling worker callback");

			// item.worker_callback(item.model_index);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
