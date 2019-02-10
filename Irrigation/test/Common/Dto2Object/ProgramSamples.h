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
					setName("name1").
					setSchedulerType(SchedulerType::PERIODIC).
					setPeriodicScheduler(PeriodicSchedulerSample1().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample1().getObject()).
					setRunTimeContainer(RunTimeListSample1().getContainer()).
					setStartTimeContainer(StartTimeListSample1().getContainer()).
					build(),
				ProgramDTO(
					"name1", to_string(SchedulerType::PERIODIC),
					PeriodicSchedulerDTO(PeriodicSchedulerSample1().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample1().getDto()),
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
					setName("name2").
					setSchedulerType(SchedulerType::WEEKLY).
					setPeriodicScheduler(PeriodicSchedulerSample2().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample2().getObject()).
					setRunTimeContainer(RunTimeListSample2().getContainer()).
					setStartTimeContainer(StartTimeListSample2().getContainer()).
					build(),
				ProgramDTO("name2", to_string(SchedulerType::WEEKLY),
					PeriodicSchedulerDTO(PeriodicSchedulerSample2().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample2().getDto()),
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
					setName("name3").
					setSchedulerType(SchedulerType::PERIODIC).
					setPeriodicScheduler(PeriodicSchedulerSample3().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample3().getObject()).
					setRunTimeContainer(RunTimeListSample3().getContainer()).
					setStartTimeContainer(StartTimeListSample3().getContainer()).
					build(),
				ProgramDTO(
					"name3", to_string(SchedulerType::PERIODIC),
					PeriodicSchedulerDTO(PeriodicSchedulerSample3().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample3().getDto()),
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
					setName("name4").
					setSchedulerType(SchedulerType::WEEKLY).
					setPeriodicScheduler(PeriodicSchedulerSample4().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample4().getObject()).
					setRunTimeContainer(RunTimeListSample4().getContainer()).
					setStartTimeContainer(StartTimeListSample4().getContainer()).
					build(),
				ProgramDTO(
					"name4", to_string(SchedulerType::WEEKLY),
					PeriodicSchedulerDTO(PeriodicSchedulerSample4().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample4().getDto()),
					std::list<RunTimeDTO>(RunTimeListSample4().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample4().getDtoList())
				)
			)
		{}
	};
};
