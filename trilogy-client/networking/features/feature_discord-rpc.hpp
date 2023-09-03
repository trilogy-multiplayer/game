#pragma once

#ifndef TRILOGY_NETWORKING_FEATURE_DISCORD_RPC_H
#define TRILOGY_NETWORKING_FEATURE_DISCORD_RPC_H

#include <common.hpp>

#include <memory/memory.hpp>
#include <random>

#include <vendor/discord/discord_rpc.hpp>
#pragma comment(lib, "vendor/discord/lib/discord-rpc.lib")

namespace networking::features {
	class c_feature_discord_rpc : public c_singleton<c_feature_discord_rpc> {
	public:
		void initialize();
		void shutdown();

		void update_rich_presence();
	private:
		bool m_running;

		void on_discord_rpc_thread();
	};
}

#endif
