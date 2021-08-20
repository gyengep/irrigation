#pragma once
#include <memory>
#include "Logic/Program.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
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
					setEnabled(false).
					setName("name1").
					setAdjustment(54).
					setSchedulerType(SchedulerType::EVERY_DAY).
					setHotWeatherScheduler(HotWeatherSchedulerSample1().getObject()).
					setTemperatureDependentScheduler(TemperatureDependentSchedulerSample1().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample1().getObject()).
					setRunTimeContainer(RunTimeListSample1().getContainer()).
					setStartTimeContainer(StartTimeListSample1().getContainer()).
					build(),
				ProgramDTO(
					false, "name1", 54, to_string(SchedulerType::EVERY_DAY),
					EveryDaySchedulerDTO(),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSample1().getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample1().getDto()),
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
					setEnabled(true).
					setName("name2").
					setAdjustment(61).
					setSchedulerType(SchedulerType::WEEKLY).
					setHotWeatherScheduler(HotWeatherSchedulerSample2().getObject()).
					setTemperatureDependentScheduler(TemperatureDependentSchedulerSample2().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample2().getObject()).
					setRunTimeContainer(RunTimeListSample2().getContainer()).
					setStartTimeContainer(StartTimeListSample2().getContainer()).
					build(),
				ProgramDTO(true, "name2", 61, to_string(SchedulerType::WEEKLY),
					EveryDaySchedulerDTO(),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSample2().getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample2().getDto()),
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
					setEnabled(false).
					setName("name3").
					setAdjustment(159).
					setSchedulerType(SchedulerType::WEEKLY).
					setHotWeatherScheduler(HotWeatherSchedulerSample3().getObject()).
					setTemperatureDependentScheduler(TemperatureDependentSchedulerSample3().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample3().getObject()).
					setRunTimeContainer(RunTimeListSample3().getContainer()).
					setStartTimeContainer(StartTimeListSample3().getContainer()).
					build(),
				ProgramDTO(
					false, "name3", 159, to_string(SchedulerType::WEEKLY),
					EveryDaySchedulerDTO(),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSample3().getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample3().getDto()),
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
					setEnabled(true).
					setName("name4").
					setAdjustment(238).
					setSchedulerType(SchedulerType::WEEKLY).
					setHotWeatherScheduler(HotWeatherSchedulerSample4().getObject()).
					setTemperatureDependentScheduler(TemperatureDependentSchedulerSample4().getObject()).
					setWeeklyScheduler(WeeklySchedulerSample4().getObject()).
					setRunTimeContainer(RunTimeListSample4().getContainer()).
					setStartTimeContainer(StartTimeListSample4().getContainer()).
					build(),
				ProgramDTO(
					true, "name4", 238, to_string(SchedulerType::WEEKLY),
					EveryDaySchedulerDTO(),
					HotWeatherSchedulerDTO(HotWeatherSchedulerSample4().getDto()),
					TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample4().getDto()),
					WeeklySchedulerDTO(WeeklySchedulerSample4().getDto()),
					std::list<RunTimeDTO>(RunTimeListSample4().getDtoList()),
					std::list<StartTimeDTO>(StartTimeListSample4().getDtoList())
				)
			)
		{}
	};
};
