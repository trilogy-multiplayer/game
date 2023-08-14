#pragma once

#ifndef TRILOGY_CORE_H
#define TRILOGY_CORE_H

#include <common.hpp>

class c_server_core : public c_singleton<c_server_core> {
public:
	void initialize();
};

#endif
