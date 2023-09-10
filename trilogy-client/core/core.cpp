#include "core.hpp"
#include <filesystem>
#include <iostream>

std::string c_core::get_computer_name() {
	const int buffer_size = MAX_COMPUTERNAME_LENGTH + 1;

	char buffer[buffer_size];
	DWORD lpnSize = buffer_size;

	if (GetComputerNameA(buffer, &lpnSize) == FALSE)
		throw std::runtime_error("Something went wrong.");

	return std::string{ buffer };
}

std::string c_core::get_trilogy_base_path()
{
	printf("Path: %s\n", std::filesystem::current_path().string().c_str());
	return std::filesystem::current_path().string().c_str();
}

std::string c_core::make_trilogy_path(std::string trilogy_path)
{
	return c_log::Join(get_trilogy_base_path(), trilogy_path);
}

