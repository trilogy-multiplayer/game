#include "core.hpp"

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
	/**
	  * TODO:
	  * Our launcher needs to set a registry value so the game knows our trilogy-path for libs.
	  */

	if (get_computer_name().find("DESKTOP-6RZ5T9") != std::string::npos)
		return m_path;

	return "";
}

