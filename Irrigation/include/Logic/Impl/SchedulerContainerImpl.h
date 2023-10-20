#pragma once
#include <memory>
#include <string>
#include "DTO/SchedulersDTO.h"
#include "Logic/SchedulerContainer.h"


class SchedulerContainerImpl : public SchedulerContainer {
	SchedulerContainer::container_type container;

	EveryDaySchedulerPtr everyDayScheduler;
	HotWeatherSchedulerPtr hotWeatherScheduler;
	TemperatureDependentSchedulerPtr temperatureDependentScheduler;
	WeeklySchedulerPtr weeklyScheduler;

public:
	SchedulerContainerImpl(
			const EveryDaySchedulerPtr& everyDayScheduler,
			const HotWeatherSchedulerPtr& hotWeatherScheduler,
			const TemperatureDependentSchedulerPtr& temperatureDependentScheduler,
			const WeeklySchedulerPtr& weeklyScheduler
		);
	virtual ~SchedulerContainerImpl() = default;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	virtual const_mapped_type at(const key_type& key) const override;	// throws NoSuchElementException
	virtual mapped_type at(const key_type& key) override;				// throws NoSuchElementException

	virtual const EveryDayScheduler& getEveryDayScheduler() const override;
	virtual const HotWeatherScheduler& getHotWeatherScheduler() const override;
	virtual const TemperatureDependentScheduler& getTemperatureDependentScheduler() const override;
	virtual const WeeklyScheduler& getWeeklyScheduler() const override;

	virtual EveryDayScheduler& getEveryDayScheduler() override;
	virtual HotWeatherScheduler& getHotWeatherScheduler() override;
	virtual TemperatureDependentScheduler& getTemperatureDependentScheduler() override;
	virtual WeeklyScheduler& getWeeklyScheduler() override;

	virtual SchedulersDTO toSchedulersDto() const override;
	virtual void updateFromSchedulersDto(const SchedulersDTO& schedulersDto) override;

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class SchedulerContainerImplFactory : public SchedulerContainerFactory {
	EveryDaySchedulerFactoryPtr everyDaySchedulerFactory;
	HotWeatherSchedulerFactoryPtr hotWeatherSchedulerFactory;
	TemperatureDependentSchedulerFactoryPtr temperatureDependentSchedulerFactory;
	WeeklySchedulerFactoryPtr weeklySchedulerFactory;

public:
	SchedulerContainerImplFactory(
			const EveryDaySchedulerFactoryPtr& everyDaySchedulerFactory,
			const HotWeatherSchedulerFactoryPtr& hotWeatherSchedulerFactory,
			const TemperatureDependentSchedulerFactoryPtr& temperatureDependentSchedulerFactory,
			const WeeklySchedulerFactoryPtr& weeklySchedulerFactory
		);
	virtual ~SchedulerContainerImplFactory() = default;
	virtual SchedulerContainerPtr create() const override;
};
