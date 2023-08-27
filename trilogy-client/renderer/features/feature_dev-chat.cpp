#include "feature_dev-chat.hpp"

#define MESSAGE_OPACITY 1.0f
#define MESSAGE_FADE_IN_OUT_TIME 150

uint64_t renderer::features::c_dev_chat::c_dev_chat_message::get_tick_count()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

uint64_t renderer::features::c_dev_chat::c_dev_chat_message::get_elapsed_time()
{
	return get_tick_count() - this->m_creation_time;
}

float renderer::features::c_dev_chat::c_dev_chat_message::get_fade_percent()
{
	const auto phase = this->get_phase();
	const auto elapsed = get_elapsed_time();

	if (phase == FADE_IN)
	{
		return ((float)elapsed / (float)MESSAGE_FADE_IN_OUT_TIME) 
			* MESSAGE_OPACITY;
	}
	else if (phase == FADE_OUT)
	{
		return (1.f - (
			((float)elapsed - (float)MESSAGE_FADE_IN_OUT_TIME - (float)this->m_duration) / 
			(float)MESSAGE_FADE_IN_OUT_TIME
		)) * MESSAGE_OPACITY;
	}

	return 1.f * MESSAGE_OPACITY;
}

renderer::features::c_dev_chat::e_dev_chat_message_phase renderer::features::c_dev_chat::c_dev_chat_message::get_phase()
{
	const auto elapsed = get_elapsed_time();

	if (elapsed > MESSAGE_FADE_IN_OUT_TIME + this->m_duration + MESSAGE_FADE_IN_OUT_TIME)
		return EXPIRED;
	else if (elapsed > MESSAGE_FADE_IN_OUT_TIME + this->m_duration)
		return FADE_OUT;
	else if (elapsed > MESSAGE_FADE_IN_OUT_TIME)
		return WAIT;
	else return FADE_IN;
}

void renderer::features::c_dev_chat::on_tick()
{
	static auto imgui_render = c_imgui_render::instance();
	static auto row_size = 14;

	for (auto i = 0; i < m_chat_messages.size(); i++)
	{
		auto* current_message = &m_chat_messages[i];

		if (current_message->get_phase() == e_dev_chat_message_phase::EXPIRED)
		{
			m_chat_messages.erase(m_chat_messages.begin() + i);
			continue;
		}

		imgui_render->render_text(current_message->m_message, ImVec2(25, 55 + (row_size * i)), 13.0f, RGBA(255, 255, 255, current_message->get_fade_percent() * 255), false, true);
	}
}
