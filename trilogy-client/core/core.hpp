#pragma once

#ifndef TRILOGY_CORE_H
#define TRILOGY_CORE_H

#include <common.hpp>

class c_core : public c_singleton<c_core> {
public:
	std::string m_name = "trilogy:MP";
	double m_version = 0.1;

	void initialize();

	const char* get_build_date();
};

#endif
