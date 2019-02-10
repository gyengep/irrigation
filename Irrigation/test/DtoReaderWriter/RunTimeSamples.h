#pragma once
#include <string>
#include "DTO/RunTimeDTO.h"

#define RUNTIME_SAMPLE_1 runTimeSample_all
#define RUNTIME_SAMPLE_2 runTimeSample_minute
#define RUNTIME_SAMPLE_3 runTimeSample_second
#define RUNTIME_SAMPLE_4 runTimeSample_id
#define RUNTIME_SAMPLE_5 runTimeSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, RunTimeDTO> RunTimeSample;

///////////////////////////////////////////////////////////////////////////////

const RunTimeSample runTimeSample_all(
		"<runtime id=\"547\"><minute>52</minute><second>43</second></runtime>",
		RunTimeDTO(52, 43).setId(547)
		);

const RunTimeSample runTimeSample_minute(
		"<runtime><minute>17</minute></runtime>",
		RunTimeDTO().setMinutes(17)
		);

const RunTimeSample runTimeSample_second(
		"<runtime><second>34</second></runtime>",
		RunTimeDTO().setSeconds(34)
		);

const RunTimeSample runTimeSample_id(
		"<runtime id=\"9348\"/>",
		RunTimeDTO().setId(9348)
		);

const RunTimeSample runTimeSample_empty(
		"<runtime/>",
		RunTimeDTO()
		);
