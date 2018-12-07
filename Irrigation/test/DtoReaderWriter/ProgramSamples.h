#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/ProgramDTO.h"
#include "RunTimeSamples.h"
#include "StartTimeSamples.h"
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

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, ProgramDTO> ProgramSample;

///////////////////////////////////////////////////////////////////////////////

const ProgramSample programSample_all(
		"<program id=\"123\">"
			"<name>abcdefg</name>"
			"<schedulertype>weekly</schedulertype>"
			"<schedulers>" +
				PERIODIC_SCHEDULER_SAMPLE_1.first +
				WEEKLY_SCHEDULER_SAMPLE_1.first +
			"</schedulers>"
			"<runtimes>" +
				RUNTIME_SAMPLE_1.first +
				RUNTIME_SAMPLE_2.first +
				RUNTIME_SAMPLE_3.first +
				RUNTIME_SAMPLE_4.first +
				RUNTIME_SAMPLE_5.first +
			"</runtimes>"
			"<starttimes>" +
				STARTTIME_SAMPLE_1.first +
				STARTTIME_SAMPLE_2.first +
				STARTTIME_SAMPLE_3.first +
				STARTTIME_SAMPLE_4.first +
				STARTTIME_SAMPLE_5.first +
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setId(123)
		.setName("abcdefg")
		.setSchedulerType("weekly")
		.setPeriodicScheduler(PeriodicSchedulerDTO(PERIODIC_SCHEDULER_SAMPLE_1.second))
		.setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_1.second))
		.setRunTimes(std::list<RunTimeDTO>({
				RUNTIME_SAMPLE_1.second,
				RUNTIME_SAMPLE_2.second,
				RUNTIME_SAMPLE_3.second,
				RUNTIME_SAMPLE_4.second,
				RUNTIME_SAMPLE_5.second
			}))
		.setStartTimes(std::list<StartTimeDTO>({
				STARTTIME_SAMPLE_1.second,
				STARTTIME_SAMPLE_2.second,
				STARTTIME_SAMPLE_3.second,
				STARTTIME_SAMPLE_4.second,
				STARTTIME_SAMPLE_5.second,
			}))
		);


const ProgramSample programSample_name(
		"<program>"
			"<name>abcdefg</name>"
		"</program>",
		ProgramDTO().setName("abcdefg")
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
				PERIODIC_SCHEDULER_SAMPLE_2.first +
			"</schedulers>"
		"</program>",
		ProgramDTO()
		.setPeriodicScheduler(PeriodicSchedulerDTO(PERIODIC_SCHEDULER_SAMPLE_2.second))
		);

const ProgramSample programSample_weeklyScheduler(
		"<program>"
			"<schedulers>" +
				WEEKLY_SCHEDULER_SAMPLE_2.first +
			"</schedulers>"
		"</program>",
		ProgramDTO()
		.setWeeklyScheduler(WeeklySchedulerDTO(WEEKLY_SCHEDULER_SAMPLE_2.second))
		);

const ProgramSample programSample_runTimes(
		"<program>"
			"<runtimes>" +
				RUNTIME_SAMPLE_3.first +
				RUNTIME_SAMPLE_4.first +
				RUNTIME_SAMPLE_5.first +
			"</runtimes>"
		"</program>",
		ProgramDTO()
		.setRunTimes(std::list<RunTimeDTO>({
				RUNTIME_SAMPLE_3.second,
				RUNTIME_SAMPLE_4.second,
				RUNTIME_SAMPLE_5.second,
			}))
		);

const ProgramSample programSample_startTimes(
		"<program>"
			"<starttimes>" +
				STARTTIME_SAMPLE_3.first +
				STARTTIME_SAMPLE_4.first +
				STARTTIME_SAMPLE_5.first +
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setStartTimes(std::list<StartTimeDTO>({
				STARTTIME_SAMPLE_3.second,
				STARTTIME_SAMPLE_4.second,
				STARTTIME_SAMPLE_5.second
			}))
		);

const ProgramSample programSample_id(
		"<program id=\"94\"/>",
		ProgramDTO().setId(94)
		);

const ProgramSample programSample_empty(
		"<program/>",
		ProgramDTO()
		);
