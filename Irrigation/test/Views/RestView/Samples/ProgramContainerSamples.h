#pragma once
#include <list>
#include <memory>
#include <vector>
#include "ProgramSamples.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class ProgramContainerSample : public SampleList<ProgramDto> {
		const std::string xmlWithoutContainers;

	public:
		ProgramContainerSample(const ProgramDtoList& programDtoList, const std::string& xml, const std::string& xmlWithoutContainers) :
			SampleList<ProgramDto>(programDtoList, xml),
			xmlWithoutContainers(xmlWithoutContainers)
		{
		}

		const std::string getXmlWithoutContainers() const {
			return xmlWithoutContainers;
		}
	};

	class ProgramContainerSampleList : public std::vector<ProgramContainerSample> {
	public:
		ProgramContainerSampleList() {

			emplace_back(
				ProgramDtoList({
				}),
				"<programs />",
				"<programs />"
			);

			emplace_back(
				ProgramDtoList({
					ProgramSampleList().at(0).getDto().setId(10)
				}),
				"<programs>" +
					ProgramSampleList().at(0).getXmlWithId(10) +
				"</programs>",
				"<programs>" +
					ProgramSampleList().at(0).getXmlWithoutContainers(10) +
				"</programs>"
			);

			emplace_back(
				ProgramDtoList({
					ProgramSampleList().at(0).getDto().setId(100),
					ProgramSampleList().at(1).getDto().setId(101),
					ProgramSampleList().at(2).getDto().setId(102)
				}),
				"<programs>" +
					ProgramSampleList().at(0).getXmlWithId(100) +
					ProgramSampleList().at(1).getXmlWithId(101) +
					ProgramSampleList().at(2).getXmlWithId(102) +
				"</programs>",
				"<programs>" +
					ProgramSampleList().at(0).getXmlWithoutContainers(100) +
					ProgramSampleList().at(1).getXmlWithoutContainers(101) +
					ProgramSampleList().at(2).getXmlWithoutContainers(102) +
				"</programs>"
			);

			emplace_back(
				ProgramDtoList({
					ProgramSampleList().at(3).getDto().setId(200),
					ProgramSampleList().at(2).getDto().setId(300),
					ProgramSampleList().at(0).getDto().setId(400),
					ProgramSampleList().at(1).getDto().setId(500)
				}),
				"<programs>" +
					ProgramSampleList().at(3).getXmlWithId(200) +
					ProgramSampleList().at(2).getXmlWithId(300) +
					ProgramSampleList().at(0).getXmlWithId(400) +
					ProgramSampleList().at(1).getXmlWithId(500) +
				"</programs>",
				"<programs>" +
					ProgramSampleList().at(3).getXmlWithoutContainers(200) +
					ProgramSampleList().at(2).getXmlWithoutContainers(300) +
					ProgramSampleList().at(0).getXmlWithoutContainers(400) +
					ProgramSampleList().at(1).getXmlWithoutContainers(500) +
				"</programs>"
			);
		}
	};
};
