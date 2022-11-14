#include "DurationList.h"
#include <sstream>
#include <iomanip>


DurationList::DurationList(std::initializer_list<std::chrono::seconds> initializer) :
	std::list<std::chrono::seconds>(initializer)
{
}

DurationList::DurationList(const std::list<std::chrono::seconds>& durations) :
	std::list<std::chrono::seconds>(durations)
{
}

DurationList::DurationList(const std::list<std::chrono::milliseconds>& durationMillis) {
	for (const auto& ms : durationMillis) {
		push_back(std::chrono::duration_cast<std::chrono::seconds>(ms));
	}
}

DurationList DurationList::adjust(unsigned adjustment) const {
	DurationList result;

	for (const auto& duration : *this) {
		result.emplace_back(std::chrono::seconds(duration * adjustment / 100));
	}

	return result;
}

std::string DurationList::toString() const {
	std::ostringstream oss;

	oss << "[";
	for (auto it = begin(); end() != it; ++it) {
		if (it != begin()) {
			oss << ", ";
		}

		oss << std::setfill('0') << std::setw(2) << (it->count() / 60) << ":";
		oss << std::setfill('0') << std::setw(2) << (it->count() % 60);
	}
	oss << "]";

	return oss.str();
}
