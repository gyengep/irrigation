#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "DTO/SchedulersDto.h"
#include "SampleBase.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<SchedulersDto> SchedulersSample;

	class SchedulersSampleList : public std::vector<SchedulersSample> {
	public:
		SchedulersSampleList() {

			emplace_back(
				SchedulersDto(),
				"<schedulers/>"
			);

			emplace_back(
				SchedulersDto().setEveryDayScheduler(EveryDaySchedulerSample().getDto()),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
				"</schedulers>"
			);

			for (const auto& hotWeatherSchedulerSample : HotWeatherSchedulerSampleList()) {
				emplace_back(
					SchedulersDto().setHotWeatherScheduler(hotWeatherSchedulerSample.getDto()),
					"<schedulers>" +
						hotWeatherSchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			for (const auto& temperatureDependentSchedulerSample : TemperatureDependentSchedulerSampleList()) {
				emplace_back(
					SchedulersDto().setTemperatureDependentScheduler(temperatureDependentSchedulerSample.getDto()),
					"<schedulers>" +
						temperatureDependentSchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			for (const auto& weeklySchedulerSample : WeeklySchedulerSampleList()) {
				emplace_back(
					SchedulersDto().setWeeklyScheduler(weeklySchedulerSample.getDto()),
					"<schedulers>" +
						weeklySchedulerSample.getXml() +
					"</schedulers>"
				);
			}

			emplace_back(
				SchedulersDto(
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
