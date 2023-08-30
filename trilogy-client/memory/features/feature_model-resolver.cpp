#include "feature_model-resolver.hpp"

void memory::features::c_model_resolver::initialize()
{
	m_model_resolve_thread = std::thread([this] { this->on_model_resolve_thread(); });
	m_model_resolve_thread.detach();
}

void memory::features::c_model_resolver::add_model_to_worker(int32_t model_index, std::function<void(int32_t)> worker_callback)
{
	std::lock_guard<std::mutex> lock(m_worker_mutex);
	m_worker_items.push({ model_index, worker_callback });
}

void memory::features::c_model_resolver::on_model_resolve_thread()
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
			sdk::api::sdk_streaming_api::instance()->load_model(item.model_index);
			item.worker_callback(item.model_index);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
