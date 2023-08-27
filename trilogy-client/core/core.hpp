#pragma once

#ifndef TRILOGY_CORE_H
#define TRILOGY_CORE_H

#include <common.hpp>

class c_core : public c_singleton<c_core> {
private:
	std::string m_path = std::string("E:\\Coding\\trilogy-mp\\x64\\Release\\");

public:
	std::string get_computer_name();
	std::string get_trilogy_base_path();

	std::string make_trilogy_path(std::string trilogy_path);
};

#endif
