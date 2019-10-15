#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/EveryDaySchedulerDTO.h"

#define EVERY_DAY_SCHEDULER_SAMPLE everyDaySchedulerSample

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, EveryDaySchedulerDTO> EveryDaySchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const EveryDaySchedulerSample everyDaySchedulerSample(
		"<scheduler type=\"every-day\"/>",
		EveryDaySchedulerDTO()
		);
