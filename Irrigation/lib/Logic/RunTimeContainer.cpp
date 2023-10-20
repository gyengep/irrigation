#include "RunTimeContainer.h"


DurationList RunTimeContainer::toDurationList(const RunTimeDtoList& runTimeDtoList) {
	DurationList result;

	for (auto it = runTimeDtoList.begin(); runTimeDtoList.end() != it; ++it) {
		result.push_back(RunTime::toDuration(*it));
	}

	return result;
}
