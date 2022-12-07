#pragma once
#include <list>
#include <string>
#include <vector>
#include "DTO/ProgramDTO.h"
#include "SampleBase.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "SchedulersSamples.h"


namespace Dto2XmlTest {

	typedef Sample<ProgramDTO> ProgramSample;

	class ProgramSampleList : public std::vector<ProgramSample> {
	public:
		ProgramSampleList() {

			emplace_back(
				ProgramDTO(),
				"<program/>"
			);

			emplace_back(
				ProgramDTO().setId(94),
				"<program id=\"94\"/>"
			);

			emplace_back(
				ProgramDTO().setName("abcdefg"),
				"<program>"
					"<name>abcdefg</name>"
				"</program>"
			);

			emplace_back(
				ProgramDTO().setEnabled(true),
				"<program>"
					"<enabled>true</enabled>"
				"</program>"
			);

			emplace_back(
				ProgramDTO().setAdjustment(57),
				"<program>"
					"<adjustment>57</adjustment>"
				"</program>"
			);

			emplace_back(
				ProgramDTO().setSchedulerType("weekly"),
				"<program>"
					"<schedulertype>weekly</schedulertype>"
				"</program>"
			);

			for (const auto& runTimeListSample : RunTimeListSampleList()) {
				emplace_back(
					ProgramDTO().setRunTimes(runTimeListSample.getDtoList()),
					"<program>" +
						runTimeListSample.getXml() +
					"</program>"
				);
			}

			for (const auto& startTimeListSample : StartTimeListSampleList()) {
				emplace_back(
					ProgramDTO().setStartTimes(startTimeListSample.getDtoList()),
					"<program>" +
						startTimeListSample.getXml() +
					"</program>"
				);
			}

			for (const auto& schedulersSample : SchedulersSampleList()) {
				emplace_back(
					ProgramDTO().setSchedulers(schedulersSample.getDto()),
					"<program>" +
						schedulersSample.getXml() +
					"</program>"
				);
			}

			emplace_back(
				ProgramDTO(false, "Qwert", 183, "every-day",
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
				"</program>"
			);
		}
	};
};
