#include "RunTimeContainer.h"


std::list<std::chrono::seconds> RunTimeContainer::toDurations(const std::list<RunTimeDTO>& runTimeDtoList) {
	std::list<std::chrono::seconds> result;

	for (auto it = runTimeDtoList.begin(); runTimeDtoList.end() != it; ++it) {
		result.push_back(RunTime::toDuration(*it));
	}

	return result;
}
