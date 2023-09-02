#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_RENDERING_FEATURE_DEV_CHAT_H
#define TRILOGY_RENDERING_FEATURE_DEV_CHAT_H

#include <common.hpp>

namespace renderer::features {
	class c_dev_chat : public c_singleton<c_dev_chat> {
	public:
		enum e_dev_chat_message_phase
		{
			FADE_IN,
			WAIT,
			FADE_OUT,
			EXPIRED,
			COUNT
		};

		class c_dev_chat_message {
		public:
			c_dev_chat_message(std::string sender, std::string message, uint32_t duration)
			{
				this->m_sender = sender;
				this->m_message = message;

				this->m_creation_time = GetTickCount64();
				this->m_duration = duration;
			}


			std::string m_sender;
			std::string m_message;

			uint64_t m_creation_time = 0;
			uint32_t m_duration = 3000;

			bool m_is_code_message = true;

			uint64_t get_tick_count();
			uint64_t get_elapsed_time();

			float get_fade_percent();
			e_dev_chat_message_phase get_phase();
		};

		std::vector<c_dev_chat_message> m_chat_messages{};
	public:
		void on_tick();
	};
}
#endif