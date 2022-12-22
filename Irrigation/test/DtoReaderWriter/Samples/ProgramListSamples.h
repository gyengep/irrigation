#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "ProgramSamples.h"


namespace DtoReaderWriterTestSamples {

	class ProgramListSample : public ContainerSample<ProgramDTO> {
		const std::string xmlWithoutContainers;

	public:
		ProgramListSample(const std::list<ProgramDTO>& dtoList, const std::string& xml, const std::string& xmlWithoutContainers) :
			ContainerSample<ProgramDTO>(dtoList, xml),
			xmlWithoutContainers(xmlWithoutContainers)
			{
			}

		const std::string& getXmlWithoutContainers() const { return xmlWithoutContainers; }
	};

	class ProgramListSampleList : public std::vector<ProgramListSample> {
	public:
		ProgramListSampleList() {

			emplace_back(
				std::list<ProgramDTO> {
				},
				"<programs/>",
				"<programs/>"
			);

			for (const auto& programSample : ProgramSampleList()) {
				emplace_back(
					std::list<ProgramDTO> {
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
				std::list<ProgramDTO> programDtoList;
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
