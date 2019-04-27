#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/WeeklySchedulerDTO.h"

#define WEEKLY_SCHEDULER_SAMPLE_1 weeklySchedulerSample_all
#define WEEKLY_SCHEDULER_SAMPLE_3 weeklySchedulerSample_values
#define WEEKLY_SCHEDULER_SAMPLE_4 weeklySchedulerSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, WeeklySchedulerDTO> WeeklySchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const WeeklySchedulerSample weeklySchedulerSample_all(
		"<scheduler type=\"weekly\">"
			"<days>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>true</day>"
			"</days>"
		"</scheduler>",
		WeeklySchedulerDTO(move(std::list<bool>({ true, false, false, true, true, false, true })))
		);

const WeeklySchedulerSample weeklySchedulerSample_values(
		"<scheduler type=\"weekly\">"
			"<days>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
			"</days>"
		"</scheduler>",
		WeeklySchedulerDTO().setValues(std::list<bool>({ true, true, true, false, true, false, false }))
		);

const WeeklySchedulerSample weeklySchedulerSample_empty(
		"<scheduler type=\"weekly\"/>",
		WeeklySchedulerDTO()
		);
