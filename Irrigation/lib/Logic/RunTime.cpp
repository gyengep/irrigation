#include "RunTime.h"


std::chrono::seconds RunTime::toDuration(const RunTimeDto& runTimeDto) {
	std::chrono::seconds result(0);

	if (runTimeDto.hasMinutes()) {
		result += std::chrono::minutes(runTimeDto.getMinutes());
	}

	if (runTimeDto.hasSeconds()) {
		result += std::chrono::seconds(runTimeDto.getSeconds());
	}

	return result;
}
