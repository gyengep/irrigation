#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/PeriodicSchedulerDTO.h"

#define PERIODIC_SCHEDULER_SAMPLE_1 periodicSchedulerSample_all
#define PERIODIC_SCHEDULER_SAMPLE_3 periodicSchedulerSample_values
#define PERIODIC_SCHEDULER_SAMPLE_4 periodicSchedulerSample_date
#define PERIODIC_SCHEDULER_SAMPLE_5 periodicSchedulerSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, PeriodicSchedulerDTO> PeriodicSchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const PeriodicSchedulerSample periodicSchedulerSample_all(
		"<scheduler type=\"periodic\">"
			"<days>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>true</day>"
			"</days>"
			"<periodStartDate>"
				"<year>2018</year>"
				"<month>11</month>"
				"<day>16</day>"
			"</periodStartDate>"
		"</scheduler>",
		PeriodicSchedulerDTO(
				std::list<bool>({ true, false, false, true, true }),
				2018, 11, 16)
		);

const PeriodicSchedulerSample periodicSchedulerSample_values(
		"<scheduler type=\"periodic\">"
			"<days>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
			"</days>"
		"</scheduler>",
		PeriodicSchedulerDTO().setValues(std::list<bool>({ true, true, true, false, false }))
		);

const PeriodicSchedulerSample periodicSchedulerSample_date(
		"<scheduler type=\"periodic\">"
			"<periodStartDate>"
				"<year>2018</year>"
				"<month>11</month>"
				"<day>16</day>"
			"</periodStartDate>"
		"</scheduler>",
		PeriodicSchedulerDTO().setPeriodStartYear(2018).setPeriodStartMonth(11).setPeriodStartDay(16)
		);

const PeriodicSchedulerSample periodicSchedulerSample_empty(
		"<scheduler type=\"periodic\"/>",
		PeriodicSchedulerDTO()
		);
