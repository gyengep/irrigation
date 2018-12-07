#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/DocumentDTO.h"
#include "ProgramSamples.h"

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, DocumentDTO> DocumentSample;

///////////////////////////////////////////////////////////////////////////////

const DocumentSample documentSample_all(
		"<irrigation>"
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
			"</programs>"
		"</irrigation>",
		DocumentDTO(std::list<ProgramDTO>({
			ProgramDTO(PROGRAM_SAMPLE_1.second),
			ProgramDTO(PROGRAM_SAMPLE_2.second),
			ProgramDTO(PROGRAM_SAMPLE_3.second),
			ProgramDTO(PROGRAM_SAMPLE_4.second),
			ProgramDTO(PROGRAM_SAMPLE_5.second),
			ProgramDTO(PROGRAM_SAMPLE_6.second),
			ProgramDTO(PROGRAM_SAMPLE_7.second),
			ProgramDTO(PROGRAM_SAMPLE_8.second),
			ProgramDTO(PROGRAM_SAMPLE_9.second),
			}))
		);


const DocumentSample documentSample_empty(
		"<irrigation/>",
		DocumentDTO()
		);
