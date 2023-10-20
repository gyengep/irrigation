#pragma once
#include <list>
#include <string>
#include <vector>
#include "DTO/ProgramDto.h"
#include "SampleBase.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "SchedulersSamples.h"


namespace DtoReaderWriterTestSamples {

	class ProgramSample : public Sample<ProgramDto> {
		const std::string xmlWithoutContainers;

	public:
		ProgramSample(const ProgramDto& dto, const std::string& xml, const std::string& xmlWithoutContainers) :
			Sample<ProgramDto>(dto, xml),
			xmlWithoutContainers(xmlWithoutContainers)
			{
			}

		const std::string& getXmlWithoutContainers() const { return xmlWithoutContainers; }
	};

	class ProgramSampleList : public std::vector<ProgramSample> {
	public:
		ProgramSampleList() {

			emplace_back(
				ProgramDto(),
				"<program/>",
				"<program/>"
			);

			emplace_back(
				ProgramDto().setId(94),
				"<program id=\"94\"/>",
				"<program id=\"94\"/>"
			);

			emplace_back(
				ProgramDto().setName("abcdefg"),
				"<program>"
					"<name>abcdefg</name>"
				"</program>",
				"<program>"
					"<name>abcdefg</name>"
				"</program>"
			);

			emplace_back(
				ProgramDto().setEnabled(true),
				"<program>"
					"<enabled>true</enabled>"
				"</program>",
				"<program>"
					"<enabled>true</enabled>"
				"</program>"
			);

			emplace_back(
				ProgramDto().setAdjustment(57),
				"<program>"
					"<adjustment>57</adjustment>"
				"</program>",
				"<program>"
					"<adjustment>57</adjustment>"
				"</program>"
			);

			emplace_back(
				ProgramDto().setSchedulerType("weekly"),
				"<program>"
					"<schedulertype>weekly</schedulertype>"
				"</program>",
				"<program>"
					"<schedulertype>weekly</schedulertype>"
				"</program>"
			);

			for (const auto& runTimeListSample : RunTimeListSampleList()) {
				emplace_back(
					ProgramDto().setRunTimes(runTimeListSample.getDtoList()),
					"<program>" +
						runTimeListSample.getXml() +
					"</program>",
					"<program/>"
				);
			}

			for (const auto& startTimeListSample : StartTimeListSampleList()) {
				emplace_back(
					ProgramDto().setStartTimes(startTimeListSample.getDtoList()),
					"<program>" +
						startTimeListSample.getXml() +
					"</program>",
					"<program/>"
				);
			}

			for (const auto& schedulersSample : SchedulersSampleList()) {
				emplace_back(
					ProgramDto().setSchedulers(schedulersSample.getDto()),
					"<program>" +
						schedulersSample.getXml() +
					"</program>",
					"<program/>"
				);
			}

			emplace_back(
				ProgramDto(false, "Qwert", 183, "every-day",
					SchedulersSampleList().back().getDto(),
					RunTimeListSampleList().back().getDtoList(),
					StartTimeListSampleList().back().getDtoList()
				).setId(123),
				"<program id=\"123\">"
					"<name>Qwert</name>"
					"<enabled>false</enabled>"
					"<adjustment>183</adjustment>"
					"<schedulertype>every-day</schedulertype>" +
					SchedulersSampleList().back().getXml() +
					RunTimeListSampleList().back().getXml() +
					StartTimeListSampleList().back().getXml() +
				"</program>",
				"<program id=\"123\">"
					"<name>Qwert</name>"
					"<enabled>false</enabled>"
					"<adjustment>183</adjustment>"
					"<schedulertype>every-day</schedulertype>"
				"</program>"
			);
		}
	};
};
