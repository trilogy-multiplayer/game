#include "core.hpp"
#include <filesystem>
#include <iostream>


std::string c_core::get_trilogy_base_path() {
	return std::filesystem::current_path().string().c_str();
}

std::string c_core::make_trilogy_path(std::string trilogy_path)
{
	return c_log::Join(get_trilogy_base_path(), trilogy_path);
}

