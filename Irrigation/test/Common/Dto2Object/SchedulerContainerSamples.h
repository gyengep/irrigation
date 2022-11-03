#pragma once
#include <memory>
#include "Logic/SchedulerContainerImpl.h"
#include "Logic/SchedulerContainerImplBuilder.h"
#include "HotWeatherSchedulerSamples.h"
#include "TemperatureDependentSchedulerSamples.h"
#include "WeeklySchedulerSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	class SchedulerContainerSample : public ObjectSample<SchedulerContainer, SchedulersDTO> {
	public:
		SchedulerContainerSample(const std::shared_ptr<SchedulerContainer>& objectPtr, SchedulersDTO&& dto) : ObjectSample<SchedulerContainer, SchedulersDTO>(objectPtr, std::move(dto)) {}
		virtual std::shared_ptr<SchedulerContainerImpl::Builder> createBuilder() const = 0;
	};


	class SchedulerContainerSample1 : public SchedulerContainerSample {
	public:
		SchedulerContainerSample1() : SchedulerContainerSample(
			createBuilder()->build(),
			SchedulersDTO(
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample1().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample1().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample1().getDto())
			)
		) {}

		virtual std::shared_ptr<SchedulerContainerImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<SchedulerContainerImpl::Builder>();

			builder->
				setHotWeatherScheduler(HotWeatherSchedulerSample1().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample1().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample1().getObjectPtr());

			return builder;
		}
	};

	class SchedulerContainerSample2 : public SchedulerContainerSample {
	public:
		SchedulerContainerSample2() : SchedulerContainerSample(
			createBuilder()->build(),
			SchedulersDTO(
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample2().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample2().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample2().getDto())
			)
		) {}

		virtual std::shared_ptr<SchedulerContainerImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<SchedulerContainerImpl::Builder>();

			builder->
				setHotWeatherScheduler(HotWeatherSchedulerSample2().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample2().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample2().getObjectPtr());

			return builder;
		}
	};

	class SchedulerContainerSample3 : public SchedulerContainerSample {
	public:
		SchedulerContainerSample3() : SchedulerContainerSample(
			createBuilder()->build(),
			SchedulersDTO(
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample3().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample3().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample3().getDto())
			)
		) {}

		virtual std::shared_ptr<SchedulerContainerImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<SchedulerContainerImpl::Builder>();

			builder->
				setHotWeatherScheduler(HotWeatherSchedulerSample3().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample3().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample3().getObjectPtr());

			return builder;
		}
	};

	class SchedulerContainerSample4 : public SchedulerContainerSample {
	public:
		SchedulerContainerSample4() : SchedulerContainerSample(
			createBuilder()->build(),
			SchedulersDTO(
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(HotWeatherSchedulerSample4().getDto()),
				TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerSample4().getDto()),
				WeeklySchedulerDTO(WeeklySchedulerSample4().getDto())
			)
		) {}

		virtual std::shared_ptr<SchedulerContainerImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<SchedulerContainerImpl::Builder>();

			builder->
				setHotWeatherScheduler(HotWeatherSchedulerSample4().getObjectPtr()).
				setTemperatureDependentScheduler(TemperatureDependentSchedulerSample4().getObjectPtr()).
				setWeeklyScheduler(WeeklySchedulerSample4().getObjectPtr());

			return builder;
		}
	};
};
