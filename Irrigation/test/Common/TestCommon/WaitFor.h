#pragma once
#include <chrono>
#include <thread>


#define WAIT_FOR(waitTime)  { 											\
	const auto endTime = std::chrono::steady_clock::now() + waitTime;	\
																		\
	auto currentTime = std::chrono::steady_clock::now();				\
	while (currentTime < endTime) {										\
		std::this_thread::sleep_for(endTime - currentTime);				\
		currentTime = std::chrono::steady_clock::now();					\
	}																	\
}
