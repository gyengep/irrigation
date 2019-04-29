#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/ProgramDTO.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "PeriodicSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"

#define PROGRAM_SAMPLE_1 programSample_all
#define PROGRAM_SAMPLE_2 programSample_name
#define PROGRAM_SAMPLE_3 programSample_schedulerType
#define PROGRAM_SAMPLE_4 programSample_periodicScheduler
#define PROGRAM_SAMPLE_5 programSample_weeklyScheduler
#define PROGRAM_SAMPLE_6 programSample_runTimes
#define PROGRAM_SAMPLE_7 programSample_startTimes
#define PROGRAM_SAMPLE_8 programSample_id
#define PROGRAM_SAMPLE_9 programSample_empty
#define PROGRAM_SAMPLE_10 programSample_disabled
#define PROGRAM_SAMPLE_11 programSample_adjustment

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, ProgramDTO> ProgramSample;

///////////////////////////////////////////////////////////////////////////////

const ProgramSample programSample_all(
		"<program id=\"123\">"
			"<name>abcdefg</name>"
			"<disabled>true</disabled>"
			"<adjustment>183</adjustment>"
			"<schedulertype>weekly</schedulertype>"
			"<schedulers>" +
				PERIODIC_SCHEDULER_SAMPLE_1.first +
				WEEKLY_SCHEDULER_SAMPLE_1.first +
			"</schedulers>" +
			RUNTIME_LIST_SAMPLE_1.first +
			STARTTIME_LIST_SAMPLE_1.first +
		"</program>",
		ProgramDTO()
		.setId(123)
		.setDisabled(true)
		.setName("abcdefg")
		.setAdjustment(183)
		.setSchedulerType("weekly")
		.setPeriodicScheduler(PeriodicSchedulerDTO(PERIODIC_SCHEDULER_SAMPLE_1.second))
		.setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_1.second))
		.setRunTimes(std::list<RunTimeDTO>(RUNTIME_LIST_SAMPLE_1.second))
		.setStartTimes(std::list<StartTimeDTO>(STARTTIME_LIST_SAMPLE_1.second))
		);


const ProgramSample programSample_disabled(
		"<program>"
			"<disabled>true</disabled>"
		"</program>",
		ProgramDTO().setDisabled(true)
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

const ProgramSample programSample_periodicScheduler(
		"<program>"
			"<schedulers>" +
				PERIODIC_SCHEDULER_SAMPLE_3.first +
			"</schedulers>"
		"</program>",
		ProgramDTO().setPeriodicScheduler(PeriodicSchedulerDTO(PERIODIC_SCHEDULER_SAMPLE_3.second))
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
