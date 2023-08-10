#pragma once

static inline float Offset(float x, float y)
{
	if (x > y)
		return x - y;

	return y - x;
}

class sdk_matrix
{
public:
	char pad_0x0000[0x30]; //0x0000
	float pos_x; //0x0030 
	float pos_y; //0x0034 
	float pos_z; //0x0038 
	char pad_0x003C[0xC]; //0x003C

	void set_position(sdk_vec3_t pos) {
		pos_x = pos.x;
		pos_y = pos.y;
		pos_z = pos.z;
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
	float m_vec_speed_x; //0x006C 
	float m_vec_speed_y; //0x0070 
	float m_vec_speed_z; //0x0074 
	char pad_0x0078[0x7C8]; //0x0078

	void update_position(sdk_vec3_t targetPosition, float time_step) {
		sdk_vec3_t vector(0, 0, 0);
		vector.x = Offset(targetPosition.x, m_matrix->pos_x);
		vector.y = Offset(targetPosition.y, m_matrix->pos_y);
		vector.z = Offset(targetPosition.z, m_matrix->pos_z);

		if (vector.x > 2.0F || vector.y > 2.0F || vector.z > 1.0F) {
			m_matrix->pos_x = targetPosition.x;
			m_matrix->pos_y = targetPosition.y;
			m_matrix->pos_z = targetPosition.z;
		}
		else
		{
			m_vec_speed_x += (targetPosition.x - m_matrix->pos_x) * 0.1F;
			m_vec_speed_y += (targetPosition.y - m_matrix->pos_y) * 0.1F;
			m_vec_speed_z += (targetPosition.z - m_matrix->pos_z) * 0.1F;

			m_matrix->pos_x += time_step * m_vec_speed_x;
			m_matrix->pos_y += time_step * m_vec_speed_y;
			m_matrix->pos_z += time_step * m_vec_speed_z;
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

