#pragma once
#include "DTO/SchedulersDto.h"


namespace DtoSamples {

	const SchedulersDto schedulersDtoSample1(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(98, 39.1f),
			TemperatureDependentSchedulerDto(0.12f, 43, 45),
			WeeklySchedulerDto(std::list<bool>{ true, false, true, false, false, true, true })
		);

	const SchedulersDto schedulersDtoSample2(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(108, 49.2f),
			TemperatureDependentSchedulerDto(0.12f, 43, 45),
			WeeklySchedulerDto(std::list<bool>{ false, false, true, true, false, false, false })
		);

	const SchedulersDto schedulersDtoSample3(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(118, 59.2f),
			TemperatureDependentSchedulerDto(3.14f, 50, 60),
			WeeklySchedulerDto(std::list<bool>{ true, false, false, true, true, true, false })
		);

}
