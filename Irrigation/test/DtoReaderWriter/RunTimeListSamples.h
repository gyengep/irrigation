#pragma once
#include <list>
#include <string>
#include "RunTimeSamples.h"

#define RUNTIME_LIST_SAMPLE_1 runTimeListSample_more
#define RUNTIME_LIST_SAMPLE_2 runTimeListSample_one
#define RUNTIME_LIST_SAMPLE_3 runTimeListSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, std::list<RunTimeDTO>> RunTimeListSample;

///////////////////////////////////////////////////////////////////////////////

const RunTimeListSample runTimeListSample_more(
		"<runtimes>" +
			RUNTIME_SAMPLE_1.first +
			RUNTIME_SAMPLE_2.first +
			RUNTIME_SAMPLE_3.first +
			RUNTIME_SAMPLE_4.first +
			RUNTIME_SAMPLE_5.first +
		"</runtimes>",
		std::list<RunTimeDTO>({
			RunTimeDTO(RUNTIME_SAMPLE_1.second),
			RunTimeDTO(RUNTIME_SAMPLE_2.second),
			RunTimeDTO(RUNTIME_SAMPLE_3.second),
			RunTimeDTO(RUNTIME_SAMPLE_4.second),
			RunTimeDTO(RUNTIME_SAMPLE_5.second)
			})
		);

const RunTimeListSample runTimeListSample_one(
		"<runtimes>" +
			RUNTIME_SAMPLE_3.first +
		"</runtimes>",
		std::list<RunTimeDTO>({
			RunTimeDTO(RUNTIME_SAMPLE_3.second)
			})
		);

const RunTimeListSample runTimeListSample_empty(
		"<runtimes/>",
		std::list<RunTimeDTO>()
		);
