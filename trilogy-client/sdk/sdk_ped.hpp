#pragma once

static inline float get_offset_from_float(float x, float y)
{
	if (x > y)
		return x - y;

	return y - x;
}

class sdk_matrix
{
public:
	char pad_0x0000[0x30]; //0x0000
	sdk_vec3_t m_position; //0x0030 
	char pad_0x003C[0xC]; //0x003C

	void set_position(sdk_vec3_t pos) {
		m_position = pos;
	}
}; //Size=0x0048

class sdk_ped
{
public:
	char pad_0x0000[0x18]; //0x0000
	sdk_matrix* m_matrix; //0x0018 
	char pad_0x0020[0x40]; //0x0020
	float m_unk1; //0x0060 
	float m_unk2; //0x0064 
	float m_unk3; //0x0068 
	sdk_vec3_t m_vec_speed;
	char pad_0x0078[0x7C8]; //0x0078

	void update_position(sdk_vec3_t target_position) {
		auto position = m_matrix->m_position;

		if (position.x != target_position.x || position.y != target_position.y || position.z != target_position.z)
		{
			sdk_vec3_t offset_position {
				get_offset_from_float(target_position.x, position.x),
				get_offset_from_float(target_position.y, position.y),
				get_offset_from_float(target_position.z, position.z)
			};

			if (offset_position.x > 7.5F || offset_position.y > 7.5F || offset_position.z > 0.5F)
				m_matrix->m_position = target_position;
			else
			{
				m_vec_speed.x += (target_position.x - position.x) * 0.15F;
				m_vec_speed.y += (target_position.y - position.y) * 0.15F;
				m_vec_speed.z += (target_position.z - position.z) * 0.15F;
			}
		}
	}

}; //Size=0x0840

/*
class sdk_player_ped
{
public:
	char pad_0x0000[0x18]; //0x0000
	sdk_matrix* m_matrix; //0x0018
	sdk_unknown* m_unknown; //0x0020
	char pad_0x0028[0x40]; //0x0028
	float m_unk0; //0x0068
	float m_vec_speed_x; //0x006C
	float m_vec_speed_y; //0x0070
	float m_vec_speed_z; //0x0074
	char pad_0x0078[0x4]; //0x0078


	*/
	//Size=0x007C

