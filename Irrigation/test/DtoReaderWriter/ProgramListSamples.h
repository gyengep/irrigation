#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/DocumentDTO.h"
#include "ProgramSamples.h"

#define PROGRAM_LIST_SAMPLE_1 programListSample_more
#define PROGRAM_LIST_SAMPLE_2 programListSample_one
#define PROGRAM_LIST_SAMPLE_3 programListSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, std::list<ProgramDTO>> ProgramListSample;

///////////////////////////////////////////////////////////////////////////////

const ProgramListSample programListSample_more(
		"<programs>" +
			PROGRAM_SAMPLE_1.first +
			PROGRAM_SAMPLE_2.first +
			PROGRAM_SAMPLE_3.first +
			PROGRAM_SAMPLE_4.first +
			PROGRAM_SAMPLE_5.first +
			PROGRAM_SAMPLE_6.first +
			PROGRAM_SAMPLE_7.first +
			PROGRAM_SAMPLE_8.first +
			PROGRAM_SAMPLE_9.first +
			PROGRAM_SAMPLE_10.first +
			PROGRAM_SAMPLE_11.first +
		"</programs>",
		std::list<ProgramDTO>({
			ProgramDTO(PROGRAM_SAMPLE_1.second),
			ProgramDTO(PROGRAM_SAMPLE_2.second),
			ProgramDTO(PROGRAM_SAMPLE_3.second),
			ProgramDTO(PROGRAM_SAMPLE_4.second),
			ProgramDTO(PROGRAM_SAMPLE_5.second),
			ProgramDTO(PROGRAM_SAMPLE_6.second),
			ProgramDTO(PROGRAM_SAMPLE_7.second),
			ProgramDTO(PROGRAM_SAMPLE_8.second),
			ProgramDTO(PROGRAM_SAMPLE_9.second),
			ProgramDTO(PROGRAM_SAMPLE_10.second),
			ProgramDTO(PROGRAM_SAMPLE_11.second),
			})
		);


const ProgramListSample programListSample_one(
		"<programs>" +
			PROGRAM_SAMPLE_2.first +
		"</programs>",
		std::list<ProgramDTO>({
			ProgramDTO(PROGRAM_SAMPLE_2.second)
			})
		);


const ProgramListSample programListSample_empty(
		"<programs/>",
		std::list<ProgramDTO>()
		);
