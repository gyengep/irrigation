#pragma once
#include "SchedulerContainerImpl.h"


class SchedulerContainerImpl::Builder {
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;

public:
	Builder();

	Builder& setEveryDayScheduler(const std::shared_ptr<EveryDayScheduler>& everyDayScheduler);
	Builder& setHotWeatherScheduler(const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler);
	Builder& setTemperatureDependentScheduler(const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler);
	Builder& setWeeklyScheduler(const std::shared_ptr<WeeklyScheduler>& weeklyScheduler);

	SchedulerContainerPtr build();
};

///////////////////////////////////////////////////////////////////////////////

class SchedulerContainerImplFactory::Builder {
	std::shared_ptr<EveryDaySchedulerFactory> everyDaySchedulerFactory;
	std::shared_ptr<HotWeatherSchedulerFactory> hotWeatherSchedulerFactory;
	std::shared_ptr<TemperatureDependentSchedulerFactory> temperatureDependentSchedulerFactory;
	std::shared_ptr<WeeklySchedulerFactory> weeklySchedulerFactory;

public:
	Builder();

	Builder& setEveryDaySchedulerFactory(const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory);
	Builder& setHotWeatherSchedulerFactory(const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory);
	Builder& setTemperatureDependentSchedulerFactory(const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory);
	Builder& setWeeklySchedulerFactory(const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory);

	SchedulerContainerFactoryPtr build();
};
