#pragma once
#include <memory>
#include "Logic/Program.h"
#include "PeriodicSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<Program, ProgramDTO> ProgramSample;


	class ProgramSample1 : public ProgramSample {
	public:
		ProgramSample1() : ProgramSample(
				Program::Builder().
					setDisabled(true).
					setName("name1").
					setAdjustment(54).
					setSchedulerType(SchedulerType::PERIODIC).
					setPeriodicScheduler(PeriodicSchedulerSample1().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample1().getObject()).
					setRunTimeContainer(RunTimeListSample1().getContainer()).
					setStartTimeContainer(StartTimeListSample1().getContainer()).
					build(),
				ProgramDTO(
					true, "name1", 54, to_string(SchedulerType::PERIODIC),
					PeriodicSchedulerDTO(PeriodicSchedulerSample1().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample1().getDto()),
					EveryDaySchedulerDTO(),
					std::list<RunTimeDTO>(RunTimeListSample1().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample1().getDtoList())
				)
			)
		{}
	};

	class ProgramSample2 : public ProgramSample {
	public:
		ProgramSample2() : ProgramSample(
				Program::Builder().
					setDisabled(false).
					setName("name2").
					setAdjustment(61).
					setSchedulerType(SchedulerType::WEEKLY).
					setPeriodicScheduler(PeriodicSchedulerSample2().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample2().getObject()).
					setRunTimeContainer(RunTimeListSample2().getContainer()).
					setStartTimeContainer(StartTimeListSample2().getContainer()).
					build(),
				ProgramDTO(false, "name2", 61, to_string(SchedulerType::WEEKLY),
					PeriodicSchedulerDTO(PeriodicSchedulerSample2().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample2().getDto()),
					EveryDaySchedulerDTO(),
					std::list<RunTimeDTO>(RunTimeListSample2().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample2().getDtoList())
				)
			)
		{}
	};

	class ProgramSample3 : public ProgramSample {
	public:
		ProgramSample3() : ProgramSample(
				Program::Builder().
					setDisabled(true).
					setName("name3").
					setAdjustment(159).
					setSchedulerType(SchedulerType::PERIODIC).
					setPeriodicScheduler(PeriodicSchedulerSample3().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample3().getObject()).
					setRunTimeContainer(RunTimeListSample3().getContainer()).
					setStartTimeContainer(StartTimeListSample3().getContainer()).
					build(),
				ProgramDTO(
					true, "name3", 159, to_string(SchedulerType::PERIODIC),
					PeriodicSchedulerDTO(PeriodicSchedulerSample3().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample3().getDto()),
					EveryDaySchedulerDTO(),
					std::list<RunTimeDTO>(RunTimeListSample3().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample3().getDtoList())
				)
			)
		{}
	};

	class ProgramSample4 : public ProgramSample {
	public:
		ProgramSample4() : ProgramSample(
				Program::Builder().
					setDisabled(false).
					setName("name4").
					setAdjustment(238).
					setSchedulerType(SchedulerType::WEEKLY).
					setPeriodicScheduler(PeriodicSchedulerSample4().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample4().getObject()).
					setRunTimeContainer(RunTimeListSample4().getContainer()).
					setStartTimeContainer(StartTimeListSample4().getContainer()).
					build(),
				ProgramDTO(
					false, "name4", 238, to_string(SchedulerType::WEEKLY),
					PeriodicSchedulerDTO(PeriodicSchedulerSample4().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample4().getDto()),
					EveryDaySchedulerDTO(),
					std::list<RunTimeDTO>(RunTimeListSample4().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample4().getDtoList())
				)
			)
		{}
	};
};
