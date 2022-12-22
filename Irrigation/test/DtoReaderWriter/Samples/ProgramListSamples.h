#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "ProgramSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef ContainerSample<ProgramDTO> ProgramListSample;

	class ProgramListSampleList : public std::vector<ProgramListSample> {
	public:
		ProgramListSampleList() {

			emplace_back(
				std::list<ProgramDTO> {
				},
				"<programs/>"
			);

			for (const auto& programSample : ProgramSampleList()) {
				emplace_back(
					std::list<ProgramDTO> {
						programSample.getDto()
					},
					"<programs>" +
						programSample.getXml() +
					"</programs>"
				);
			}

			{
				std::list<ProgramDTO> programDtoList;
				std::string xml;

				for (const auto& programSample : ProgramSampleList()) {
					programDtoList.push_back(programSample.getDto());
					xml.append(programSample.getXml());
				}

				emplace_back(
					programDtoList,
					"<programs>" +
						xml +
					"</programs>"
				);
			}
		}
	};
};
