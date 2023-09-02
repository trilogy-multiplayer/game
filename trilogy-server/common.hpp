#pragma once

/**
 * trilogy:MP
 * Copyright (c) 2023 PARADOX
 */

#ifndef TRILOGY_SERVER_COMMON_H
#define TRILOGY_SERVER_COMMON_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <thread>
#include <chrono>

#include "utilities/singleton.hpp"
#include "utilities/logging.hpp"

#include <definitions.hpp>

#endif