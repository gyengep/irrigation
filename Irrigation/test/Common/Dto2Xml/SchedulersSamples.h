#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "DTO/SchedulersDTO.h"
#include "SampleBase.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"


namespace Dto2XmlTest {

	typedef Sample<SchedulersDTO> SchedulersSample;

	class SchedulersSampleList : public std::vector<SchedulersSample> {
	public:
		SchedulersSampleList() {

			emplace_back(
				SchedulersDTO(),
				"<schedulers/>"
			);

			emplace_back(
				SchedulersDTO().setEveryDayScheduler(EveryDaySchedulerSample().getDto()),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
				"</schedulers>"
			);

			for (const auto& hotWeatherSchedulerSample : HotWeatherSchedulerSampleList()) {
				emplace_back(
					SchedulersDTO().setHotWeatherScheduler(hotWeatherSchedulerSample.getDto()),
					"<schedulers>" +
						hotWeatherSchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			for (const auto& temperatureDependentSchedulerSample : TemperatureDependentSchedulerSampleList()) {
				emplace_back(
					SchedulersDTO().setTemperatureDependentScheduler(temperatureDependentSchedulerSample.getDto()),
					"<schedulers>" +
						temperatureDependentSchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			for (const auto& weeklySchedulerSample : WeeklySchedulerSampleList()) {
				emplace_back(
					SchedulersDTO().setWeeklyScheduler(weeklySchedulerSample.getDto()),
					"<schedulers>" +
						weeklySchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			emplace_back(
				SchedulersDTO(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().back().getDto(),
					TemperatureDependentSchedulerSampleList().back().getDto(),
					WeeklySchedulerSampleList().back().getDto()
				),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
					HotWeatherSchedulerSampleList().back().getXml() +
					TemperatureDependentSchedulerSampleList().back().getXml() +
					WeeklySchedulerSampleList().back().getXml() +
				"</schedulers>"
			);
		}
	};
};
