#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/ProgramDTO.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "EveryDaySchedulerSamples.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"

#define PROGRAM_SAMPLE_1 programSample_all
#define PROGRAM_SAMPLE_2 programSample_name
#define PROGRAM_SAMPLE_3 programSample_schedulerType
#define PROGRAM_SAMPLE_5 programSample_weeklyScheduler
#define PROGRAM_SAMPLE_6 programSample_runTimes
#define PROGRAM_SAMPLE_7 programSample_startTimes
#define PROGRAM_SAMPLE_8 programSample_id
#define PROGRAM_SAMPLE_9 programSample_empty
#define PROGRAM_SAMPLE_10 programSample_disabled
#define PROGRAM_SAMPLE_11 programSample_adjustment
#define PROGRAM_SAMPLE_12 programSample_everyDayScheduler
#define PROGRAM_SAMPLE_13 programSample_hotWeatherScheduler
#define PROGRAM_SAMPLE_14 programSample_temperatureDependentScheduler

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, ProgramDTO> ProgramSample;

///////////////////////////////////////////////////////////////////////////////

const ProgramSample programSample_all(
		"<program id=\"123\">"
			"<name>abcdefg</name>"
			"<enabled>false</enabled>"
			"<adjustment>183</adjustment>"
			"<schedulertype>weekly</schedulertype>"
			"<schedulers>" +
				EVERY_DAY_SCHEDULER_SAMPLE.first +
				HOT_WEATHER_SCHEDULER_SAMPLE_1.first +
				TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.first +
				WEEKLY_SCHEDULER_SAMPLE_1.first +
			"</schedulers>" +
			RUNTIME_LIST_SAMPLE_1.first +
			STARTTIME_LIST_SAMPLE_1.first +
		"</program>",
		ProgramDTO()
		.setId(123)
		.setEnabled(false)
		.setName("abcdefg")
		.setAdjustment(183)
		.setSchedulerType("weekly")
		.setEveryDayScheduler(EveryDaySchedulerDTO(EVERY_DAY_SCHEDULER_SAMPLE.second))
		.setHotWeatherScheduler(HotWeatherSchedulerDTO(HOT_WEATHER_SCHEDULER_SAMPLE_1.second))
		.setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.second))
		.setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_1.second))
		.setRunTimes(std::list<RunTimeDTO>(RUNTIME_LIST_SAMPLE_1.second))
		.setStartTimes(std::list<StartTimeDTO>(STARTTIME_LIST_SAMPLE_1.second))
		);


const ProgramSample programSample_disabled(
		"<program>"
			"<enabled>false</enabled>"
		"</program>",
		ProgramDTO().setEnabled(false)
		);

const ProgramSample programSample_name(
		"<program>"
			"<name>abcdefg</name>"
		"</program>",
		ProgramDTO().setName("abcdefg")
		);

const ProgramSample programSample_adjustment(
		"<program>"
			"<adjustment>57</adjustment>"
		"</program>",
		ProgramDTO().setAdjustment(57)
		);

const ProgramSample programSample_schedulerType(
		"<program>"
			"<schedulertype>weekly</schedulertype>"
		"</program>",
		ProgramDTO().setSchedulerType("weekly")
		);

const ProgramSample programSample_everyDayScheduler(
		"<program>"
			"<schedulers>" +
				EVERY_DAY_SCHEDULER_SAMPLE.first +
			"</schedulers>"
		"</program>",
		ProgramDTO().setEveryDayScheduler(EveryDaySchedulerDTO(EVERY_DAY_SCHEDULER_SAMPLE.second))
		);

const ProgramSample programSample_hotWeatherScheduler(
		"<program>"
			"<schedulers>" +
				HOT_WEATHER_SCHEDULER_SAMPLE_2.first +
			"</schedulers>"
		"</program>",
		ProgramDTO().setHotWeatherScheduler(HotWeatherSchedulerDTO(HOT_WEATHER_SCHEDULER_SAMPLE_2.second))
		);

const ProgramSample programSample_temperatureDependentScheduler(
		"<program>"
			"<schedulers>" +
				TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.first +
			"</schedulers>"
		"</program>",
		ProgramDTO().setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1.second))
		);

const ProgramSample programSample_weeklyScheduler(
		"<program>"
			"<schedulers>" +
				WEEKLY_SCHEDULER_SAMPLE_3.first +
			"</schedulers>"
		"</program>",
		ProgramDTO().setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_3.second))
		);

const ProgramSample programSample_runTimes(
		"<program>" + RUNTIME_LIST_SAMPLE_2.first + "</program>",
		ProgramDTO().setRunTimes(std::list<RunTimeDTO>(RUNTIME_LIST_SAMPLE_2.second))
		);

const ProgramSample programSample_startTimes(
		"<program>" + STARTTIME_LIST_SAMPLE_3.first + "</program>",
		ProgramDTO().setStartTimes(std::list<StartTimeDTO>(STARTTIME_LIST_SAMPLE_3.second))
		);

const ProgramSample programSample_id(
		"<program id=\"94\"/>",
		ProgramDTO().setId(94)
		);

const ProgramSample programSample_empty(
		"<program/>",
		ProgramDTO()
		);
