#define LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ENET_IMPLEMENTATION
#define LIBRG_DEBUG

#include "networking.hpp"
#include <vendor/minhook/minhook.hpp>
#include <mutex>
#include <utilities/ida.hpp>

static int32_t online_players = 5;

static int8_t get_player_id_by_ptr(int64_t this_ptr) {
	static int64_t(&sdk_world_players)[] = *(int64_t(*)[])memory::as_relative(memory::find_pattern(memory::module_t(nullptr), "world_players", "48 8B 0D ? ? ? ? 48 89 58 10 48 89 78 F0"));

	for (int8_t i = 0; i <= online_players; i++)
	{
		if (sdk_world_players[56 * i] == this_ptr)
			return i;
	}

	return -1;
}

c_player_entity* c_networking::get_player_by_id(int32_t id) {
	for (auto& entity : c_networking::instance()->m_players)
	{
		if (entity->network_id == id)
			return entity;
	}

	return nullptr;
}

c_player_entity* c_networking::get_player_by_game_id(int32_t id) {
	for (auto& entity : c_networking::instance()->m_players)
	{
		if (entity->player_id == id)
			return entity;
	}

	return nullptr;
}

std::once_flag once_flag;
static void h_sdk_player_connect(int64_t this_ptr) {
	auto networking = c_networking::instance();

	if (this_ptr == c_memory::instance()->sdk_find_player_ped(0)) {
		networking->o_sdk_player_connect(this_ptr);
		return;
	}

	auto player_id = get_player_id_by_ptr(this_ptr);
	if (player_id == -1) return;

	auto player = networking->get_player_by_game_id(player_id);
	if (player == nullptr) return;

	// sdk_player_ped* p = (sdk_player_ped*)this_ptr;
	// p->update_position(entity->position);
	// c_log::Info("(on_client_entity_update) setting", player->player_id, "at", pos.x, pos.y, pos.z);

	/*
	auto old_controls = c_memory::instance()->sdk_player_pad->OldState;
	auto controls = c_memory::instance()->sdk_player_pad->NewState;
	*c_memory::instance()->sdk_player_in_focus = player_id;

	memset(&c_memory::instance()->sdk_player_pad->OldState, 0, sizeof(CGtaControls));
	memset(&c_memory::instance()->sdk_player_pad->NewState, 0, sizeof(CGtaControls));

	networking->o_sdk_player_connect(this_ptr);

	c_memory::instance()->sdk_player_pad->OldState = old_controls;
	c_memory::instance()->sdk_player_pad->NewState = controls;

	*c_memory::instance()->sdk_player_in_focus = 0;
	*/

	hid_mapping current_hid_state = *c_memory::instance()->sdk_hid_mapping;
	auto camera_heading = *c_memory::instance()->sdk_current_camera_data_front;

	//for (size_t i = 0; i < 102; i++)
	//	c_memory::instance()->sdk_hid_mapping->m_states[i].m_cur_state = e_hid_mapping_current_state::NOT_PRESSED;

	c_memory::instance()->sdk_current_camera_data_front->offset_pos_x = player->player_sync_data->camera_front.x;
	c_memory::instance()->sdk_current_camera_data_front->offset_pos_y = player->player_sync_data->camera_front.y;
	c_memory::instance()->sdk_current_camera_data_front->offset_pos_z = player->player_sync_data->camera_front.z;

	*c_memory::instance()->sdk_hid_mapping = decompress_mapping(player->player_sync_data->mapping, current_hid_state);

	if (!player->player_sync_data->mapping.is_empty()) {
		*(_DWORD*)((__int64)this_ptr + 1428) = player->player_sync_data->ped_state;
		*(float*)((__int64)this_ptr + 1820) = player->player_sync_data->current_rotation;

		sdk_ped* ped = (sdk_ped*)this_ptr;
		ped->update_position(player->position, player->player_sync_data->move_speed, 0);
	}

	networking->o_sdk_player_connect(this_ptr);

	c_memory::instance()->sdk_current_camera_data_front->offset_pos_x = camera_heading.offset_pos_x;
	c_memory::instance()->sdk_current_camera_data_front->offset_pos_y = camera_heading.offset_pos_y;
	c_memory::instance()->sdk_current_camera_data_front->offset_pos_z = camera_heading.offset_pos_z;

	*c_memory::instance()->sdk_hid_mapping = current_hid_state;

	std::call_once(once_flag, [&] {
		c_log::Info("Controlling", (int)player_id, this_ptr);
		});
}


using sdk_ped_create_t = _QWORD * (*)(_QWORD* a1, int a2, unsigned int a3);
sdk_ped_create_t o_sdk_ped_create;

_QWORD* h_create_ped(_QWORD* a1, int a2, unsigned int a3)
{
	_QWORD* original_result = o_sdk_ped_create(a1, a2, a3);

	c_log::Info("(h)", "Create ped", a1, a2, a3);

	return original_result;
}

void c_networking::initialize() {
	auto sdk_player_connect = memory::find_pattern<sdk_player_connect_t>(memory::module_t(nullptr), "c_networking::sdk_player_connect", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 48 8D 68 A1");
	MH_CreateHook(sdk_player_connect, h_sdk_player_connect, reinterpret_cast<void**>(&o_sdk_player_connect));
	MH_EnableHook(sdk_player_connect);

	//auto sdk_ped_create = memory::find_pattern(memory::module_t(), "sdk_ped::create", "48 89 5C 24 08 57 48 83 EC ? 41 8B D8 48 8B F9 E8 ? ? ? ? 48 8D 05 ? ? ? ? 8B D3 48 8B CF 48 89 07 E8 ? ? ? ? 48 8D 8F 68 03 00 00 48 8B D7 E8 ? ? ? ? 48 8B 5C 24 30 48 8B C7 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 08");
	//MH_CreateHook((LPVOID)sdk_ped_create, h_create_ped, reinterpret_cast<void**>(&o_sdk_ped_create));
	//MH_EnableHook((LPVOID)sdk_ped_create);

	this->m_ctx = { 0 };
	this->m_is_running = false;
	this->m_is_connected = false;
}

/**
 * TODO:
 * Split this later in to class members.
 */


static void on_connect_requesting(librg_event_t* event)
{
	auto instance = c_networking::instance();
	librg_data_wstr(event->data, instance->m_client_name);

	c_log::Info("Networking", ">>", "Connecting to @::", instance->m_address.port, "as", instance->m_client_name);
}

static void on_connect_accepted(librg_event_t* event)
{
	auto instance = c_networking::instance();

	auto player = new c_player_entity(event->entity->id, instance->m_client_name, true, sdk_vec3_t(SPAWN_POS_X, SPAWN_POS_Y, SPAWN_POS_Z));
	c_log::Info("Networking", ">>", "Connected - (char_id:", player->char_id, ", player_id:", player->player_id, ")");

	event->entity->type = (int)player->entity_type;
	event->entity->user_data = player;
}

static void on_connect_refuse(librg_event_t* event)
{
	c_log::Info("Networking", ">>", "Refuse");
}

static void on_connect_disconnect(librg_event_t* event)
{
	c_log::Info("Networking", ">>", "Disconnect");
}

void on_client_streamer_update(librg_event_t* event)
{
	if (event->entity->type == (int)e_entity_types::PLAYER)
	{
		c_player_entity* player = (c_player_entity*)event->entity->user_data;
		sdk_ped* player_ped = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(0);

		if (player == nullptr || player_ped == nullptr) return;

		player->player_sync_data = new packet_player_sync_data();
		player->player_sync_data->mapping = compress_mapping(*c_memory::instance()->sdk_hid_mapping);
		player->player_sync_data->move_speed = sdk_vec3_t(player_ped->m_vec_speed_x, player_ped->m_vec_speed_y, player_ped->m_vec_speed_z);
		player->player_sync_data->camera_front = c_memory::instance()->sdk_current_camera_data_front->get_offset_pos();

		player->player_sync_data->ped_state = *(_DWORD*)((__int64)player_ped + 1428);
		player->player_sync_data->current_rotation = *(float*)((__int64)player_ped + 1820);

		c_scripting::instance()->call_opcode(sdk_script_commands::COMMAND_GET_CHAR_COORDINATES, player->char_id,
			&event->entity->position.x, &event->entity->position.y, &event->entity->position.z);

		librg_data_wptr(event->data, player->player_sync_data, sizeof(packet_player_sync_data));
	}
}

std::time_t last_update;

void on_client_entity_create(librg_event_t* event)
{
	auto p = c_networking::instance()->get_player_by_id(event->entity->id);
	if (p != nullptr) return;

	auto pos = event->entity->position;
	auto player = new c_player_entity(event->entity->id, "unknown", false, sdk_vec3_t(pos.x, pos.y, pos.z));

	c_log::Info("Entity-Create (network_id:", event->entity->id, "player_id:", player->player_id);
}


void on_client_entity_update(librg_event_t* event)
{
	auto player = c_networking::instance()->get_player_by_id(event->entity->id);
	if (player == nullptr || player->is_local) return;

	auto pos = event->entity->position;
	player->position = sdk_vec3_t(pos.x, pos.y, pos.z);

	sdk_ped* player_ped = (sdk_ped*)c_memory::instance()->sdk_find_player_ped(player->player_id);
	if (player_ped == nullptr) {
		c_log::Info("(on_client_entity_update)", "Cant find player id", player->player_id);
		return;
	}

	librg_data_rptr(event->data, player->player_sync_data, sizeof(packet_player_sync_data));
}

void on_client_entity_remove(librg_event_t* event)
{
	auto player = c_networking::instance()->get_player_by_id(event->entity->id);
	if (player == nullptr || player->is_local) return;

	c_scripting::instance()->call_opcode(COMMAND_DELETE_PLAYER,
		player->player_id
	);
}

void c_networking::on_client_thread()
{
	while (this->m_is_running)
	{
		if (GetAsyncKeyState(VK_SUBTRACT) & 0x1) {
			this->m_is_running = false;
		}

		librg_tick(&m_ctx);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	librg_network_stop(&m_ctx);
	librg_free(&m_ctx);
}

bool c_networking::connect_to(const char* address, int32_t port)
{
	this->m_is_running = true;

	m_ctx.mode = LIBRG_MODE_CLIENT;
	m_ctx.tick_delay = 16;

	librg_init(&m_ctx);

	librg_event_add(&m_ctx, LIBRG_CONNECTION_REQUEST, on_connect_requesting);
	librg_event_add(&m_ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
	librg_event_add(&m_ctx, LIBRG_CONNECTION_REFUSE, on_connect_refuse);
	librg_event_add(&m_ctx, LIBRG_CONNECTION_DISCONNECT, on_connect_disconnect);

	librg_event_add(&m_ctx, LIBRG_ENTITY_CREATE, on_client_entity_create);
	librg_event_add(&m_ctx, LIBRG_ENTITY_UPDATE, on_client_entity_update);
	librg_event_add(&m_ctx, LIBRG_ENTITY_REMOVE, on_client_entity_remove);

	librg_event_add(&m_ctx, LIBRG_CLIENT_STREAMER_UPDATE, on_client_streamer_update);

	this->m_address.host = (char*)address;
	this->m_address.port = port;

	c_log::Info("Networking", ">>", "Initialized client context.");

	librg_network_start(&m_ctx, m_address);

	static std::thread client_thread([this] { this->on_client_thread(); });

	return true;
}

