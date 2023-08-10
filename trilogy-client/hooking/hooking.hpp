#pragma once

#include "utilities/logging.hpp"
#include "utilities/singleton.hpp"

class c_hooking : public c_singleton<c_hooking> {
public:
    bool initialize();
};