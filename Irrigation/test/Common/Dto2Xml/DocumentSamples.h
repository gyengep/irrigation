#pragma once
#include <list>
#include <string>
#include <vector>
#include "DTO/DocumentDTO.h"
#include "SampleBase.h"
#include "ProgramListSamples.h"


namespace Dto2XmlTest {

	typedef Sample<DocumentDTO> DocumentSample;

	class DocumentSampleList : public std::vector<DocumentSample> {
	public:
		DocumentSampleList() {

			emplace_back(
				DocumentDTO(),
				"<irrigation/>"
			);

			for (const auto& programListSample : ProgramListSampleList()) {
				emplace_back(
					DocumentDTO().setPrograms(programListSample.getDtoList()),
					"<irrigation>" +
						programListSample.getXml() +
					"</irrigation>"
				);
			}

			for (const auto& programListSample : ProgramListSampleList()) {
				emplace_back(
					DocumentDTO(programListSample.getDtoList()),
					"<irrigation>" +
						programListSample.getXml() +
					"</irrigation>"
				);
			}
}
	};
};
