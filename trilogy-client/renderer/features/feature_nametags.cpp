#include "feature_nametags.hpp"

void renderer::features::c_nametags::on_tick() {
	static auto networking = c_networking::instance();
	static auto memory = c_memory::instance();
	static auto imgui_render = c_imgui_render::instance();

	/*for (auto& player : networking->m_players) {
		sdk_vec3_t position;
		float w, h;

		sdk_ped* player_ped = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(player->player_id);
		auto matrix = player_ped->m_matrix;

		if (memory->sdk_calc_screen_coords(sdk_vec3_t(matrix->pos_x, matrix->pos_y, matrix->pos_z + 0.8f), &position, &w, &h, true, true)) {
			imgui_render->render_text(player->player_sync_data->name, ImVec2(position.x, position.y), 15.0f, RGBA(255, 255, 255, 255), true);
		}
	}*/
}