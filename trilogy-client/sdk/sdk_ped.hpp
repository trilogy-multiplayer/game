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

	void update_position(sdk_vec3_t targetPosition, sdk_vec3_t vecMoveSpeed, float time_step) {
		sdk_vec3_t offset_vector(0, 0, 0);
		offset_vector.x = get_offset_from_float(targetPosition.x, m_matrix->pos_x);
		offset_vector.y = get_offset_from_float(targetPosition.y, m_matrix->pos_y);
		offset_vector.z = get_offset_from_float(targetPosition.z, m_matrix->pos_z);

		if (offset_vector.x < 0.1f && offset_vector.y < 0.1f && offset_vector.z < 0.1f) return;
			
			/*sdk_vec3_t vector(0, 0, 0);
			vector.x = Offset(targetPosition.x, m_matrix->pos_x);
			vector.y = Offset(targetPosition.y, m_matrix->pos_y);
			vector.z = Offset(targetPosition.z, m_matrix->pos_z);
			s
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
			}*/


		sdk_vec3_t vecPosOffset;
		sdk_vec3_t vecSpeedOffset;

		sdk_vec3_t nextPosition = targetPosition;
		sdk_vec3_t nextSpeed = vecMoveSpeed;
		sdk_vec3_t currentSpeed = sdk_vec3_t(m_vec_speed_x, m_vec_speed_y, m_vec_speed_z);
		sdk_vec3_t currentPosition = sdk_vec3_t(m_matrix->pos_x, m_matrix->pos_y, m_matrix->pos_z);

		const float fLerpFactor = 30.f;

		vecPosOffset.x = (nextPosition.x - currentPosition.x) / fLerpFactor;
		vecPosOffset.y = (nextPosition.y - currentPosition.y) / fLerpFactor;
		vecPosOffset.z = (nextPosition.z - currentPosition.z) / fLerpFactor;

		vecSpeedOffset.x = (nextSpeed.x - currentSpeed.x) / fLerpFactor;
		vecSpeedOffset.y = (nextSpeed.y - currentSpeed.y) / fLerpFactor;
		vecSpeedOffset.z = (nextSpeed.z - currentSpeed.z) / fLerpFactor;

		m_matrix->pos_x += vecPosOffset.x;
		m_matrix->pos_y += vecPosOffset.y;
		m_matrix->pos_z += vecPosOffset.z;

		m_vec_speed_x += vecSpeedOffset.x;
		m_vec_speed_y += vecSpeedOffset.y;
		m_vec_speed_z += vecSpeedOffset.z;
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

