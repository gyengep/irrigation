#pragma once
#include <memory>
#include <vector>
#include "DTO/ProgramDto.h"
#include "RunTimeContainerSamples.h"
#include "StartTimeContainerSamples.h"
#include "SchedulerContainerSamples.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class ProgramSample : public Sample<ProgramDto> {
		const std::string xmlWithoutContainers;

	public:
		ProgramSample(const ProgramDto& dto, const std::string& xml, const std::string& xmlWithoutContainers) :
			Sample<ProgramDto>(dto, xml),
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
				ProgramDto(
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
				ProgramDto(
					true, "program2", 61, to_string(SchedulerType::HOT_WEATHER),
					SchedulersDto(SchedulerContainerSampleList().at(1).getDto()),
					RunTimeDtoList(RunTimeContainerSampleList().at(1).getDtoList()),
					StartTimeDtoList(StartTimeContainerSampleList().at(1).getDtoList())
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
				ProgramDto(
					false, "program3", 159, to_string(SchedulerType::TEMPERATURE_DEPENDENT),
					SchedulersDto(SchedulerContainerSampleList().at(2).getDto()),
					RunTimeDtoList(RunTimeContainerSampleList().at(2).getDtoList()),
					StartTimeDtoList(StartTimeContainerSampleList().at(2).getDtoList())
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
				ProgramDto(
					true, "program4", 238, to_string(SchedulerType::WEEKLY),
					SchedulersDto(SchedulerContainerSampleList().at(3).getDto()),
					RunTimeDtoList(RunTimeContainerSampleList().at(3).getDtoList()),
					StartTimeDtoList(StartTimeContainerSampleList().at(3).getDtoList())
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
