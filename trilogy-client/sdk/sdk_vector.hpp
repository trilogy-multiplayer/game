#pragma once

struct sdk_vec3_t {
	float x, y, z;

	sdk_vec3_t() : x(0), y(0), z(0) {}

	sdk_vec3_t(float _x, float _y, float _z) 
		: x(_x), y(_y), z(_z) 
	{ 

	}
};

struct sdk_vec4_t {
	float w, x, y, z;

	void normalize();
	void slerp(sdk_vec4_t b, float t);

	sdk_vec4_t(float _w, float _x, float _y, float _z)
		: w(_w), x(_x), y(_y), z(_z)
	{

	}
};
