#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/StartTimeDTO.h"

#define STARTTIME_SAMPLE_1 startTimeSample_all
#define STARTTIME_SAMPLE_2 startTimeSample_hour
#define STARTTIME_SAMPLE_3 startTimeSample_minute
#define STARTTIME_SAMPLE_4 startTimeSample_id
#define STARTTIME_SAMPLE_5 startTimeSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, StartTimeDTO> StartTimeSample;

///////////////////////////////////////////////////////////////////////////////

const StartTimeSample startTimeSample_all(
		"<starttime id=\"8465\"><hour>1</hour><minute>2</minute></starttime>",
		StartTimeDTO(1, 2).setId(8465)
		);

const StartTimeSample startTimeSample_hour(
		"<starttime><hour>11</hour></starttime>",
		StartTimeDTO().setHour(11)
		);

const StartTimeSample startTimeSample_minute(
		"<starttime><minute>12</minute></starttime>",
		StartTimeDTO().setMinute(12)
		);

const StartTimeSample startTimeSample_id(
		"<starttime id=\"45152\"/>",
		StartTimeDTO().setId(45152)
		);

const StartTimeSample startTimeSample_empty(
		"<starttime/>",
		StartTimeDTO()
		);
