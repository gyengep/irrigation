#pragma once
#include <list>
#include <string>
#include "StartTimeSamples.h"

#define STARTTIME_LIST_SAMPLE_1 startTimeListSample_more
#define STARTTIME_LIST_SAMPLE_2 startTimeListSample_one
#define STARTTIME_LIST_SAMPLE_3 startTimeListSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, std::list<StartTimeDTO>> StartTimeListSample;

///////////////////////////////////////////////////////////////////////////////

const StartTimeListSample startTimeListSample_more(
		"<starttimes>" +
			STARTTIME_SAMPLE_1.first +
			STARTTIME_SAMPLE_2.first +
			STARTTIME_SAMPLE_3.first +
			STARTTIME_SAMPLE_4.first +
			STARTTIME_SAMPLE_5.first +
		"</starttimes>",
		std::list<StartTimeDTO>({
			StartTimeDTO(STARTTIME_SAMPLE_1.second),
			StartTimeDTO(STARTTIME_SAMPLE_2.second),
			StartTimeDTO(STARTTIME_SAMPLE_3.second),
			StartTimeDTO(STARTTIME_SAMPLE_4.second),
			StartTimeDTO(STARTTIME_SAMPLE_5.second)
			})
		);


const StartTimeListSample startTimeListSample_one(
		"<starttimes>" +
			STARTTIME_SAMPLE_4.first +
		"</starttimes>",
		std::list<StartTimeDTO>({
			StartTimeDTO(STARTTIME_SAMPLE_4.second)
			})
		);


const StartTimeListSample startTimeListSample_empty(
		"<starttimes/>",
		std::list<StartTimeDTO>()
		);
