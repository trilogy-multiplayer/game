#include "core.hpp"

void c_core::initialize()
{

}

const char* c_core::get_build_date()
{
	std::stringstream stream_date;
	stream_date << __DATE__ << " - " << __TIME__;

	return stream_date.str().c_str();
}
