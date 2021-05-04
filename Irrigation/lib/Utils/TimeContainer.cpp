#include "TimeContainer.h"
#include <stdexcept>


TimeContainer::TimeContainer(const std::vector<std::chrono::milliseconds>& times) :
	times(times),
	index(0)
{
	if (times.empty()) {
		throw std::invalid_argument("TimeContainer::TimeContainer() times.empty()");
	}
}

const std::chrono::milliseconds& TimeContainer::getNext() const {
	const auto& result = times[index];

	if ((index + 1) < times.size()) {
		index++;
	}

	return result;
}
