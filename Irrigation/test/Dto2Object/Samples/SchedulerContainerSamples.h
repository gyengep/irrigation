#pragma once
#include <memory>
#include <vector>
#include "DTO/SchedulersDTO.h"
#include "Logic/SchedulerContainerImpl.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<SchedulerContainerImpl, SchedulersDTO> SchedulerContainerSample;

	class SchedulerContainerSampleList : public std::vector<SchedulerContainerSample> {
	public:
		SchedulerContainerSampleList() {

			emplace_back(
				std::make_shared<SchedulerContainerImpl>(
					EveryDaySchedulerSample().getObjectPtr(),
					HotWeatherSchedulerSampleList().at(1).getObjectPtr(),
					TemperatureDependentSchedulerSampleList().at(2).getObjectPtr(),
					WeeklySchedulerSampleList().at(3).getObjectPtr()
				),
				SchedulersDTO(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(1).getDto(),
					TemperatureDependentSchedulerSampleList().at(2).getDto(),
					WeeklySchedulerSampleList().at(3).getDto()
				)
			);

			emplace_back(
				std::make_shared<SchedulerContainerImpl>(
					EveryDaySchedulerSample().getObjectPtr(),
					HotWeatherSchedulerSampleList().at(2).getObjectPtr(),
					TemperatureDependentSchedulerSampleList().at(3).getObjectPtr(),
					WeeklySchedulerSampleList().at(1).getObjectPtr()
				),
				SchedulersDTO(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(2).getDto(),
					TemperatureDependentSchedulerSampleList().at(3).getDto(),
					WeeklySchedulerSampleList().at(1).getDto()
				)
			);

			emplace_back(
				std::make_shared<SchedulerContainerImpl>(
					EveryDaySchedulerSample().getObjectPtr(),
					HotWeatherSchedulerSampleList().at(3).getObjectPtr(),
					TemperatureDependentSchedulerSampleList().at(1).getObjectPtr(),
					WeeklySchedulerSampleList().at(2).getObjectPtr()
				),
				SchedulersDTO(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(3).getDto(),
					TemperatureDependentSchedulerSampleList().at(1).getDto(),
					WeeklySchedulerSampleList().at(2).getDto()
				)
			);

			emplace_back(
				std::make_shared<SchedulerContainerImpl>(
					EveryDaySchedulerSample().getObjectPtr(),
					HotWeatherSchedulerSampleList().at(0).getObjectPtr(),
					TemperatureDependentSchedulerSampleList().at(1).getObjectPtr(),
					WeeklySchedulerSampleList().at(2).getObjectPtr()
				),
				SchedulersDTO(
					EveryDaySchedulerSample().getDto(),
					HotWeatherSchedulerSampleList().at(0).getDto(),
					TemperatureDependentSchedulerSampleList().at(1).getDto(),
					WeeklySchedulerSampleList().at(2).getDto()
				)
			);
		}
	};
};
