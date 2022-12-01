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


namespace Dto2ObjectTest {

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
					EveryDaySchedulerDTO(EveryDaySchedulerSample().getDto()),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSampleList().at(1).getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSampleList().at(2).getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSampleList().at(3).getDto())
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
					EveryDaySchedulerDTO(EveryDaySchedulerSample().getDto()),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSampleList().at(2).getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSampleList().at(3).getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSampleList().at(1).getDto())
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
					EveryDaySchedulerDTO(EveryDaySchedulerSample().getDto()),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSampleList().at(3).getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSampleList().at(1).getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSampleList().at(2).getDto())
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
					EveryDaySchedulerDTO(EveryDaySchedulerSample().getDto()),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSampleList().at(0).getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSampleList().at(1).getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSampleList().at(2).getDto())
				)
			);
		}
	};
};
