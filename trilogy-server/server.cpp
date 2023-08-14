#include <common.hpp>
#include <networking/server_networking.hpp>

int main()
{
	c_log::add_out(new c_log::c_log_consolestream);

	c_server_networking::instance()->initialize();
	std::cin.get();
}