#pragma once
#include <chrono>
#include <thread>

namespace FixedWait {

	template<typename _Rep, typename _Period>
	void wait_for(const std::chrono::duration<_Rep, _Period>& waitTime) {
		const auto endTime = std::chrono::steady_clock::now() + waitTime;

		auto currentTime = std::chrono::steady_clock::now();
		while (currentTime < endTime) {
			std::this_thread::sleep_for(endTime - currentTime);
			currentTime = std::chrono::steady_clock::now();
		}
	}

}
