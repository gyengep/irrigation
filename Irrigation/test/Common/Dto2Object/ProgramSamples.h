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

	class ProgramSample : public ObjectSample<Program, ProgramDTO> {
	public:
		ProgramSample(const std::shared_ptr<Program>& objectPtr, ProgramDTO&& dto) : ObjectSample<Program, ProgramDTO>(objectPtr, std::move(dto)) {}
		virtual std::shared_ptr<Program::Builder> createBuilder() const = 0;
	};


	class ProgramSample1 : public ProgramSample {
	public:
		ProgramSample1() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				false, "name1", 54, to_string(SchedulerType::EVERY_DAY),
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample1().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample1().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample1().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample1().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample1().getDtoList())
			)
		) {}

		virtual std::shared_ptr<Program::Builder> createBuilder() const override {
			auto builder = std::make_shared<Program::Builder>();

			builder->setEnabled(false).
				setName("name1").
				setAdjustment(54).
				setSchedulerType(SchedulerType::EVERY_DAY).
				setHotWeatherScheduler(HotWeatherSchedulerSample1().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample1().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample1().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample1().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample1().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample2 : public ProgramSample {
	public:
		ProgramSample2() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(true, "name2", 61, to_string(SchedulerType::WEEKLY),
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample2().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample2().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample2().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample2().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample2().getDtoList())
			)
		) {}

		virtual std::shared_ptr<Program::Builder> createBuilder() const override {
			auto builder = std::make_shared<Program::Builder>();

			builder->
				setEnabled(true).
				setName("name2").
				setAdjustment(61).
				setSchedulerType(SchedulerType::WEEKLY).
				setHotWeatherScheduler(HotWeatherSchedulerSample2().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample2().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample2().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample2().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample2().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample3 : public ProgramSample {
	public:
		ProgramSample3() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				false, "name3", 159, to_string(SchedulerType::WEEKLY),
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample3().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample3().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample3().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample3().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample3().getDtoList())
			)
		) {}

		virtual std::shared_ptr<Program::Builder> createBuilder() const override {
			auto builder = std::make_shared<Program::Builder>();

			builder->
				setEnabled(false).
				setName("name3").
				setAdjustment(159).
				setSchedulerType(SchedulerType::WEEKLY).
				setHotWeatherScheduler(HotWeatherSchedulerSample3().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample3().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample3().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample3().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample3().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample4 : public ProgramSample {
	public:
		ProgramSample4() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				true, "name4", 238, to_string(SchedulerType::WEEKLY),
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample4().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample4().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample4().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample4().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample4().getDtoList())
			)
		) {}

		virtual std::shared_ptr<Program::Builder> createBuilder() const override {
			auto builder = std::make_shared<Program::Builder>();

			builder->
				setEnabled(true).
				setName("name4").
				setAdjustment(238).
				setSchedulerType(SchedulerType::WEEKLY).
				setHotWeatherScheduler(HotWeatherSchedulerSample4().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample4().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample4().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample4().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample4().getContainerPtr());

			return builder;
		}
	};
};
