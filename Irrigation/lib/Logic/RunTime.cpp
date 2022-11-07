#include "RunTime.h"


std::chrono::seconds RunTime::toDuration(const RunTimeDTO& runTimeDTO) {
	std::chrono::seconds result(0);

	if (runTimeDTO.hasMinutes()) {
		result += std::chrono::minutes(runTimeDTO.getMinutes());
	}

	if (runTimeDTO.hasSeconds()) {
		result += std::chrono::seconds(runTimeDTO.getSeconds());
	}

	return result;
}
