#pragma once
#include <memory>
#include <vector>
#include "DTO/SchedulersDto.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<SchedulersDto> SchedulerContainerSample;

	class SchedulerContainerSampleList : public std::vector<SchedulerContainerSample> {
	public:
		SchedulerContainerSampleList() {

			emplace_back(
				SchedulersDto(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(1).getDto(),
					TemperatureDependentSchedulerSampleList().at(2).getDto(),
					WeeklySchedulerSampleList().at(3).getDto()
				),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
					HotWeatherSchedulerSampleList().at(1).getXml() +
					TemperatureDependentSchedulerSampleList().at(2).getXml() +
					WeeklySchedulerSampleList().at(3).getXml() +
				"</schedulers>"
			);

			emplace_back(
				SchedulersDto(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(2).getDto(),
					TemperatureDependentSchedulerSampleList().at(3).getDto(),
					WeeklySchedulerSampleList().at(1).getDto()
				),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
					HotWeatherSchedulerSampleList().at(2).getXml() +
					TemperatureDependentSchedulerSampleList().at(3).getXml() +
					WeeklySchedulerSampleList().at(1).getXml() +
				"</schedulers>"
			);

			emplace_back(
				SchedulersDto(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(3).getDto(),
					TemperatureDependentSchedulerSampleList().at(1).getDto(),
					WeeklySchedulerSampleList().at(2).getDto()
				),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
					HotWeatherSchedulerSampleList().at(3).getXml() +
					TemperatureDependentSchedulerSampleList().at(1).getXml() +
					WeeklySchedulerSampleList().at(2).getXml() +
				"</schedulers>"
			);

			emplace_back(
				SchedulersDto(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(0).getDto(),
					TemperatureDependentSchedulerSampleList().at(1).getDto(),
					WeeklySchedulerSampleList().at(2).getDto()
				),
				"<schedulers>" +
					EveryDaySchedulerSample().getXml() +
					HotWeatherSchedulerSampleList().at(0).getXml() +
					TemperatureDependentSchedulerSampleList().at(1).getXml() +
					WeeklySchedulerSampleList().at(2).getXml() +
				"</schedulers>"
			);
		}
	};
};
