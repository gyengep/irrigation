#pragma once
#include <memory>
#include <string>
#include "DTO/SchedulersDTO.h"
#include "SchedulerContainer.h"


class SchedulerContainerImpl : public SchedulerContainer {
public:
	class Builder;

private:
	SchedulerContainer::container_type container;

	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;

public:
	SchedulerContainerImpl(
			const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
			const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
			const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
			const std::shared_ptr<WeeklyScheduler>& weeklyScheduler
		);
	virtual ~SchedulerContainerImpl() = default;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	virtual const_mapped_type at(const key_type& key) const override;
	virtual mapped_type at(const key_type& key) override;

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
public:
	class Builder;

private:
	std::shared_ptr<EveryDaySchedulerFactory> everyDaySchedulerFactory;
	std::shared_ptr<HotWeatherSchedulerFactory> hotWeatherSchedulerFactory;
	std::shared_ptr<TemperatureDependentSchedulerFactory> temperatureDependentSchedulerFactory;
	std::shared_ptr<WeeklySchedulerFactory> weeklySchedulerFactory;

public:
	SchedulerContainerImplFactory(
			const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory,
			const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory,
			const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory,
			const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory
		);
	virtual ~SchedulerContainerImplFactory() = default;
	virtual SchedulerContainerPtr create() const override;
};
