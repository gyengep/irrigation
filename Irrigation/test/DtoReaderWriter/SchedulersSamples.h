#pragma once
#include <string>
#include <utility>
#include "DTO/SchedulersDTO.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"

#define SCHEDULERS_SAMPLE_1 schedulersSample_all
#define SCHEDULERS_SAMPLE_2 schedulersSample_everyDayScheduler
#define SCHEDULERS_SAMPLE_3 schedulersSample_hotWeatherScheduler
#define SCHEDULERS_SAMPLE_4 schedulersSample_temperatureDependentScheduler
#define SCHEDULERS_SAMPLE_5 schedulersSample_weeklyScheduler

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, SchedulersDTO> SchedulersSample;

///////////////////////////////////////////////////////////////////////////////

const SchedulersSample schedulersSample_all(
			"<schedulers>" +
				EVERY_DAY_SCHEDULER_SAMPLE.first +
				HOT_WEATHER_SCHEDULER_SAMPLE_1.first +
				TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.first +
				WEEKLY_SCHEDULER_SAMPLE_1.first +
			"</schedulers>",
			SchedulersDTO()
				.setEveryDayScheduler(EveryDaySchedulerDTO(EVERY_DAY_SCHEDULER_SAMPLE.second))
				.setHotWeatherScheduler(HotWeatherSchedulerDTO(HOT_WEATHER_SCHEDULER_SAMPLE_1.second))
				.setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.second))
				.setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_1.second))
		);


const SchedulersSample schedulersSample_everyDayScheduler(
		"<schedulers>" +
			EVERY_DAY_SCHEDULER_SAMPLE.first +
		"</schedulers>",
		SchedulersDTO().setEveryDayScheduler(EveryDaySchedulerDTO(EVERY_DAY_SCHEDULER_SAMPLE.second))
		);

const SchedulersSample schedulersSample_hotWeatherScheduler(
		"<schedulers>" +
			HOT_WEATHER_SCHEDULER_SAMPLE_2.first +
		"</schedulers>",
		SchedulersDTO().setHotWeatherScheduler(HotWeatherSchedulerDTO(HOT_WEATHER_SCHEDULER_SAMPLE_2.second))
		);

const SchedulersSample schedulersSample_temperatureDependentScheduler(
		"<schedulers>" +
			TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.first +
		"</schedulers>",
		SchedulersDTO().setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.second))
		);

const SchedulersSample schedulersSample_weeklyScheduler(
		"<schedulers>" +
			WEEKLY_SCHEDULER_SAMPLE_3.first +
		"</schedulers>",
		SchedulersDTO().setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_3.second))
		);

const SchedulersSample schedulersSample_empty(
		"<schedulers/>",
		SchedulersDTO()
		);
