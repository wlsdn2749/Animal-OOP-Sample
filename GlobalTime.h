#pragma once
#include <cstdint>
#include <chrono>

using namespace std::chrono;
namespace TimeUtil
{
	inline static const int64_t GetCurrentTime()
	{
		auto now = steady_clock::now();
		auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
		return ms;
	}
}
