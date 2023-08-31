#define LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ENET_IMPLEMENTATION
#define LIBRG_DEBUG

#include "networking.hpp"
#include <networking/features/feature_nickgen.hpp>

void c_networking::initialize() {
	this->m_client_name = "TRILOGY:" + networking::features::c_feature_nickgen::instance()->get_nickname();

	this->m_ctx = { 0 };
	this->m_is_running = false;
	this->m_is_connected = false;
}

void c_networking::on_connect_request(librg_event_t* librg_event)
{
	c_log::Info(c_log::LGreen, "(c_networking::on_connect_request):",
		c_log::LWhite, "Trying to establish a connection to:", m_address.host);

	librg_data_wu8(librg_event->data, TRILOGY_VERSION_MAJOR);
	librg_data_wu8(librg_event->data, TRILOGY_VERSION_MINOR);
	librg_data_wu8(librg_event->data, TRILOGY_VERSION_PATCH);
	librg_data_wu8(librg_event->data, TRILOGY_BUILD_CHANNEL);
	librg_data_wstring(librg_event->data, this->m_client_name);
}

void c_networking::on_connect_accept(librg_event_t* librg_event)
{
	c_log::Info(c_log::LGreen, "(c_networking::on_connect_accepted):",
		c_log::LWhite, "Connection completed! Requesting player spawning.");

	/**
	  * Announce player to network players
	  */
	librg_lambda_message_send_all(&m_ctx, NETWORK_PLAYER_CONNECT, [&](librg_data* data) {
		librg_data_wstring(data, m_client_name);
	});
}

#define INITIALIZE_MODULE_SYNC(class_instance)				\
	class_instance::instance()->initialize(&m_ctx);			\
	c_log::Info(c_log::LGreen, "(c_networking::init):",	    \
		c_log::LWhite, "Started hooking instance:",			\
		c_log::LCyan, #class_instance);						\

bool c_networking::connect_to(const char* address, int32_t port)
{
	this->m_is_running = true;

	m_ctx.mode = LIBRG_MODE_CLIENT;
	m_ctx.tick_delay = 16;

	librg_init(&m_ctx);

	REGISTER_LIBRG_EVENT(&m_ctx, LIBRG_CONNECTION_REQUEST, c_networking::instance()->on_connect_request);
	REGISTER_LIBRG_EVENT(&m_ctx, LIBRG_CONNECTION_ACCEPT, c_networking::instance()->on_connect_accept);

	INITIALIZE_MODULE_SYNC(networking::modules::c_module_player_sync);

	this->m_address.host = (char*)address;
	this->m_address.port = port;

	c_log::Info(c_log::LGreen, "(c_networking::connect_to):",
		c_log::LWhite, "Initialized librg context.");

	librg_network_start(&m_ctx, m_address);

	static std::thread client_thread([this] { this->on_client_thread(); });
	client_thread.detach();

	return true;
}

void c_networking::on_client_thread()
{
	while (this->m_is_running)
	{
		if (GetAsyncKeyState(VK_SUBTRACT) & 0x1) {
			this->m_is_running = false;
			break;
		}

		librg_tick(&m_ctx);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	librg_network_stop(&m_ctx);
	librg_free(&m_ctx);
}