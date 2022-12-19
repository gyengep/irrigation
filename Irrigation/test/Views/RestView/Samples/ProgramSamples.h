#pragma once
#include <memory>
#include <vector>
#include "DTO/ProgramDTO.h"
#include "RunTimeContainerSamples.h"
#include "StartTimeContainerSamples.h"
#include "SchedulerContainerSamples.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	class ProgramSample : public Sample<ProgramDTO> {
		const std::string xmlWithoutContainers;

	public:
		ProgramSample(const ProgramDTO& dto, const std::string& xml, const std::string& xmlWithoutContainers) :
			Sample<ProgramDTO>(dto, xml),
			xmlWithoutContainers(xmlWithoutContainers)
		{
		}

		const std::string getXmlWithoutContainers(unsigned id) const {
			return replaceIdInXml(xmlWithoutContainers, id);
		}
	};

	class ProgramSampleList : public std::vector<ProgramSample> {
	public:
		ProgramSampleList() {

			emplace_back(
				ProgramDTO(
					false, "program1", 54, to_string(SchedulerType::EVERY_DAY),
					SchedulerContainerSampleList().at(0).getDto(),
					RunTimeContainerSampleList().at(0).getDtoList(),
					StartTimeContainerSampleList().at(0).getDtoList()
				),
				"<program%s>"
					"<name>program1</name>"
					"<enabled>false</enabled>"
					"<adjustment>54</adjustment>"
					"<schedulertype>every-day</schedulertype>" +
					SchedulerContainerSampleList().at(0).getXml() +
					RunTimeContainerSampleList().at(0).getXml() +
					StartTimeContainerSampleList().at(0).getXml() +
				"</program>",
				"<program%s>"
					"<name>program1</name>"
					"<enabled>false</enabled>"
					"<adjustment>54</adjustment>"
					"<schedulertype>every-day</schedulertype>"
				"</program>"
			);

			emplace_back(
				ProgramDTO(
					true, "program2", 61, to_string(SchedulerType::HOT_WEATHER),
					SchedulersDTO(SchedulerContainerSampleList().at(1).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(1).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(1).getDtoList())
				),
				"<program%s>"
					"<name>program2</name>"
					"<enabled>true</enabled>"
					"<adjustment>61</adjustment>"
					"<schedulertype>hot-weather</schedulertype>" +
					SchedulerContainerSampleList().at(1).getXml() +
					RunTimeContainerSampleList().at(1).getXml() +
					StartTimeContainerSampleList().at(1).getXml() +
				"</program>",
				"<program%s>"
					"<name>program2</name>"
					"<enabled>true</enabled>"
					"<adjustment>61</adjustment>"
					"<schedulertype>hot-weather</schedulertype>"
				"</program>"
			);

			emplace_back(
				ProgramDTO(
					false, "program3", 159, to_string(SchedulerType::TEMPERATURE_DEPENDENT),
					SchedulersDTO(SchedulerContainerSampleList().at(2).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(2).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(2).getDtoList())
				),
				"<program%s>"
					"<name>program3</name>"
					"<enabled>false</enabled>"
					"<adjustment>159</adjustment>"
					"<schedulertype>temperature-dependent</schedulertype>" +
					SchedulerContainerSampleList().at(2).getXml() +
					RunTimeContainerSampleList().at(2).getXml() +
					StartTimeContainerSampleList().at(2).getXml() +
				"</program>",
				"<program%s>"
					"<name>program3</name>"
					"<enabled>false</enabled>"
					"<adjustment>159</adjustment>"
					"<schedulertype>temperature-dependent</schedulertype>"
				"</program>"
			);

			emplace_back(
				ProgramDTO(
					true, "program4", 238, to_string(SchedulerType::WEEKLY),
					SchedulersDTO(SchedulerContainerSampleList().at(3).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(3).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(3).getDtoList())
				),
				"<program%s>"
					"<name>program4</name>"
					"<enabled>true</enabled>"
					"<adjustment>238</adjustment>"
					"<schedulertype>weekly</schedulertype>" +
					SchedulerContainerSampleList().at(3).getXml() +
					RunTimeContainerSampleList().at(3).getXml() +
					StartTimeContainerSampleList().at(3).getXml() +
				"</program>",
				"<program%s>"
					"<name>program4</name>"
					"<enabled>true</enabled>"
					"<adjustment>238</adjustment>"
					"<schedulertype>weekly</schedulertype>"
				"</program>"
			);
		}
	};
};
