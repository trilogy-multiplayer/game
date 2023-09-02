#include "feature_nametags.hpp"

void renderer::features::c_nametags::on_tick() {
	static auto module_player_sync = networking::modules::c_module_player_sync::instance();
	static auto memory = c_memory::instance();
	static auto imgui_render = c_imgui_render::instance();

	/**
	  * Thanks to hydrogen.ac
	  */
	for (auto& player : module_player_sync->m_players) {
		if (player == nullptr) continue;
		if (player->m_game_player == nullptr) continue;

		sdk_vec3_t ped_head, ped_foot;
		float w, h;

		auto matrix = *player->m_game_player->m_matrix;

		if (!memory->sdk_calc_screen_coords({ matrix.m_position.x, matrix.m_position.y, matrix.m_position.z + .85f }, &ped_head, &w, &h, true, true))
			return;

		if (!memory->sdk_calc_screen_coords({ matrix.m_position.x, matrix.m_position.y, matrix.m_position.z - 1.f }, &ped_foot, &w, &h, true, true))
			return;

		auto draw_list = ImGui::GetCurrentWindow()->DrawList;

		auto box_height = ped_head.y - ped_foot.y;
		auto box_width = box_height / 4.0f;

		auto box_left = ped_head.x + box_width;
		auto box_right = ped_head.x - box_width;

		auto box_middle = floor(box_left + ((box_right - box_left) / 2));

		if (box_width > -7.5f)
			box_width = -7.5f;

			float green = (player->m_health * 0.8) * 2.55;
			ImColor health_color = ImColor((int)(255 - green), (int)green, 0);

			const auto max_height = ped_head.y - ped_foot.y;
			const float height = (float)((float)(player->m_health * 0.8) / 100);

			draw_list->AddRectFilled(
				ImVec2(floor(box_left - 1), floor(ped_foot.y + 2)), 
				ImVec2(floor(box_right + 1), floor(ped_foot.y + 7)), 
				ImColor(0, 0, 0, 255), 0);

			draw_list->AddRectFilled(
				ImVec2(floor(box_left), floor(ped_foot.y + 3)), 
				ImVec2(floor(box_left - (box_width * ((player->m_health * 0.8) * 0.01f) * 2)), floor(ped_foot.y + 6)),
				health_color, 0);

		imgui_render->render_text(
			c_log::Join(player->m_name, " (", player->m_network_id, ")"),
			{ box_middle, floor(ped_foot.y + 10.f) },
			13.0f,
			RGBA(255, 255, 255, 255),
			true, true
		);
	}
}