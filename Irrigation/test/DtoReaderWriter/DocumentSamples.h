#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/DocumentDTO.h"
#include "ProgramListSamples.h"


///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, DocumentDTO> DocumentSample;

///////////////////////////////////////////////////////////////////////////////

const DocumentSample documentSample_1(
		"<irrigation>" + PROGRAM_LIST_SAMPLE_1.first + "</irrigation>",
		DocumentDTO(std::list<ProgramDTO>(PROGRAM_LIST_SAMPLE_1.second))
		);

const DocumentSample documentSample_2(
		"<irrigation>" + PROGRAM_LIST_SAMPLE_2.first + "</irrigation>",
		DocumentDTO(std::list<ProgramDTO>(PROGRAM_LIST_SAMPLE_2.second))
		);


const DocumentSample documentSample_empty(
		"<irrigation/>",
		DocumentDTO()
		);
