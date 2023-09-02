#pragma once

#ifndef TRILOGY_BENCHMARK_H
#define TRILOGY_BENCHMARK_H

#include <common.hpp>

#include <memory/memory.hpp>

namespace networking::features {
	class c_feature_benchmark : public c_singleton<c_feature_benchmark> {
	public:
		void run_benchmark();
	};
}

#endif
