#define LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ENET_IMPLEMENTATION
#define LIBRG_DEBUG

#include "server_networking.hpp"

void c_server_networking::on_connect_request(librg_event_t* event)
{
	auto peer_ip = event->peer->address;

	char hostname[128] = { 0 };
	enet_address_get_host_ip(&peer_ip, hostname, 128);

	auto build_major = librg_data_ru8(event->data);
	auto build_minor = librg_data_ru8(event->data);
	auto build_patch = librg_data_ru8(event->data);
	auto build_channel = librg_data_ru8(event->data);
	auto client_name = librg_data_rstring(event->data);

	if (build_major != TRILOGY_VERSION_MAJOR ||
		build_minor != TRILOGY_VERSION_MINOR ||
		build_channel != TRILOGY_BUILD_CHANNEL) {
		c_log::Info(c_log::LGreen, "(c_server_networking::on_connect_request):",
			c_log::LWhite, "Declined connection because of version mismatch from:", hostname, ":", peer_ip.port);
		librg_event_reject(event);

		return;
	}

	c_log::Info(c_log::LGreen, "(c_server_networking::on_connect_request):",
		c_log::LWhite, "Incoming connection from:", c_log::Join(hostname, ":", std::to_string(peer_ip.port)), "as", client_name);
}

void c_server_networking::on_connect_accept(librg_event_t* librg_event)
{
	c_log::Info(c_log::LGreen, "(c_server_networking::on_connect_accept):",
		c_log::LWhite, "Valid connection accepted.");
}

#define INITIALIZE_MODULE_SYNC(class_instance)				\
	class_instance::instance()->initialize(&m_ctx);			\
	c_log::Info(c_log::LGreen, "(c_networking::init):",	    \
		c_log::LWhite, "Started hooking instance:",			\
		c_log::LCyan, #class_instance);						\


void c_server_networking::initialize()
{
	m_ctx.world_size = zpl_vec3f(5000.0f, 5000.0f, 5000.0f);

	m_ctx.mode = LIBRG_MODE_SERVER;
	m_ctx.tick_delay = 32;

	m_ctx.max_connections = MAX_PLAYERS;
	m_ctx.max_entities = (MAX_ENTITIES + MAX_PLAYERS);

	librg_init(&m_ctx);

	//librg_option_set(LIBRG_DEFAULT_STREAM_RANGE, 10);

	REGISTER_LIBRG_EVENT(&m_ctx, LIBRG_CONNECTION_REQUEST, c_server_networking::instance()->on_connect_request);
	REGISTER_LIBRG_EVENT(&m_ctx, LIBRG_CONNECTION_ACCEPT, c_server_networking::instance()->on_connect_accept);

	INITIALIZE_MODULE_SYNC(networking::modules::c_module_player_sync);

	c_log::Info(c_log::LGreen, "(c_server_networking::initialize):",
		c_log::LWhite, "Initialized librg context.");

	librg_address_t address = { 1337, (char*)"127.0.0.1" };
	librg_network_start(&m_ctx, address);

	c_log::Info(c_log::LGreen, "(c_server_networking::initialize):",
		c_log::LWhite, "Starting trilogy-server on:", c_log::LCyan, address.to_string());

	static std::thread client_thread([this] { this->on_client_thread(); });
	client_thread.detach();
}


void c_server_networking::on_client_thread()
{
	while (this->m_is_running)
	{
		librg_tick(&m_ctx);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	librg_network_stop(&m_ctx);
	librg_free(&m_ctx);
}