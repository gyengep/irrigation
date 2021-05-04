#pragma once
#include <chrono>
#include <vector>


class TimeContainer {
	const std::vector<std::chrono::milliseconds> times;
	mutable size_t index;

public:
	explicit TimeContainer(const std::vector<std::chrono::milliseconds>& times);

	const std::chrono::milliseconds& getNext() const;
};
