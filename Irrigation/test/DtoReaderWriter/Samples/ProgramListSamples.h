#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "ProgramSamples.h"


namespace DtoReaderWriterTestSamples {

	class ProgramListSample : public ContainerSample<ProgramDto> {
		const std::string xmlWithoutContainers;

	public:
		ProgramListSample(const ProgramDtoList& dtoList, const std::string& xml, const std::string& xmlWithoutContainers) :
			ContainerSample<ProgramDto>(dtoList, xml),
			xmlWithoutContainers(xmlWithoutContainers)
			{
			}

		const std::string& getXmlWithoutContainers() const { return xmlWithoutContainers; }
	};

	class ProgramListSampleList : public std::vector<ProgramListSample> {
	public:
		ProgramListSampleList() {

			emplace_back(
				ProgramDtoList {
				},
				"<programs/>",
				"<programs/>"
			);

			for (const auto& programSample : ProgramSampleList()) {
				emplace_back(
					ProgramDtoList {
						programSample.getDto()
					},
					"<programs>" +
						programSample.getXml() +
					"</programs>",
					"<programs>" +
						programSample.getXmlWithoutContainers() +
					"</programs>"
				);
			}

			{
				ProgramDtoList programDtoList;
				std::string xml, xmlWithoutContainers;

				for (const auto& programSample : ProgramSampleList()) {
					programDtoList.push_back(programSample.getDto());
					xml.append(programSample.getXml());
					xmlWithoutContainers.append(programSample.getXmlWithoutContainers());
				}

				emplace_back(
					programDtoList,
					"<programs>" +
						xml +
					"</programs>",
					"<programs>" +
						xmlWithoutContainers +
					"</programs>"
				);
			}
		}
	};
};
