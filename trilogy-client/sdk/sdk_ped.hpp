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

	void update_position(sdk_vec3_t targetPosition, sdk_vec3_t vecMoveSpeed, float time_step) {
		sdk_vec3_t offset_vector(0, 0, 0);
		offset_vector.x = get_offset_from_float(targetPosition.x, m_matrix->m_position.x);
		offset_vector.y = get_offset_from_float(targetPosition.y, m_matrix->m_position.y);
		offset_vector.z = get_offset_from_float(targetPosition.z, m_matrix->m_position.z);

		if (offset_vector.x < 0.1f && offset_vector.y < 0.1f && offset_vector.z < 0.1f) return;

		sdk_vec3_t nextPosition = targetPosition;
		sdk_vec3_t nextSpeed = vecMoveSpeed;
		sdk_vec3_t currentSpeed = m_vec_speed;
		sdk_vec3_t currentPosition = m_matrix->m_position;

		const float fLerpFactor = 30.f;

		sdk_vec3_t vec_pos_offset;
		vec_pos_offset.x = (nextPosition.x - currentPosition.x) / fLerpFactor;
		vec_pos_offset.y = (nextPosition.y - currentPosition.y) / fLerpFactor;
		vec_pos_offset.z = (nextPosition.z - currentPosition.z) / fLerpFactor;

		sdk_vec3_t vec_speed_offset;
		vec_speed_offset.x = (nextSpeed.x - currentSpeed.x) / fLerpFactor;
		vec_speed_offset.y = (nextSpeed.y - currentSpeed.y) / fLerpFactor;
		vec_speed_offset.z = (nextSpeed.z - currentSpeed.z) / fLerpFactor;

		m_matrix->m_position.x += vec_pos_offset.x;
		m_matrix->m_position.y += vec_pos_offset.y;
		m_matrix->m_position.z += vec_pos_offset.z;

		m_vec_speed.x += vec_speed_offset.x;
		m_vec_speed.y += vec_speed_offset.y;
		m_vec_speed.z += vec_speed_offset.z;
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

