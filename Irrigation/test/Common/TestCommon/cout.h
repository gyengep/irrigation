#pragma once
#include <chrono>
#include <ostream>


inline std::ostream& operator<<(std::ostream& os, const std::chrono::steady_clock::time_point& timePoint) {
	os << std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count();
	return os;
}


template<typename _Rep, typename _Period>
inline std::ostream& operator<<(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) {
	os << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return os;
}
