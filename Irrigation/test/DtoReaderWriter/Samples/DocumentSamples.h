#pragma once
#include <list>
#include <string>
#include <vector>
#include "DTO/DocumentDto.h"
#include "SampleBase.h"
#include "ProgramListSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<DocumentDto> DocumentSample;

	class DocumentSampleList : public std::vector<DocumentSample> {
	public:
		DocumentSampleList() {

			emplace_back(
				DocumentDto(),
				"<irrigation/>"
			);

			for (const auto& programListSample : ProgramListSampleList()) {
				emplace_back(
					DocumentDto().setPrograms(programListSample.getDtoList()),
					"<irrigation>" +
						programListSample.getXml() +
					"</irrigation>"
				);
			}

			for (const auto& programListSample : ProgramListSampleList()) {
				emplace_back(
					DocumentDto(programListSample.getDtoList()),
					"<irrigation>" +
						programListSample.getXml() +
					"</irrigation>"
				);
			}
}
	};
};
