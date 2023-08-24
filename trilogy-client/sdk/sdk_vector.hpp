#pragma once

struct sdk_vec2_t {
	float x, y;

	sdk_vec2_t() : x(0), y(0) {}

	sdk_vec2_t(float _x, float _y)
		: x(_x), y(_y)
	{

	}

	sdk_vec2_t floor() {
		x = std::floor(x);
		y = std::floor(y);

		return { x, y };
	}

	inline bool operator==(sdk_vec2_t vec) {
		return vec.x == x &&
			vec.y == y;
	}
};

struct sdk_vec3_t {
	float x, y, z;

	sdk_vec3_t() : x(0), y(0), z(0) {}

	sdk_vec3_t(float _x, float _y, float _z) 
		: x(_x), y(_y), z(_z) 
	{ 

	}

	inline bool operator==(sdk_vec3_t vec) {
		return vec.x == x &&
			vec.y == y &&
			vec.z == z;
	}

	sdk_vec3_t floor() {
		x = std::floor(x);
		y = std::floor(y);
		z = std::floor(z);

		return { x, y, z };
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
