#pragma once
#include "ProgramImpl.h"


class ProgramImpl::Builder {
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	std::shared_ptr<RunTimeContainer> runTimeContainer;
	std::shared_ptr<StartTimeContainer> startTimeContainer;
	std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	Builder();

	Builder& setEnabled(bool enabled);
	Builder& setName(const std::string& name);
	Builder& setAdjustment(unsigned adjustment);
	Builder& setSchedulerType(SchedulerType schedulerType);
	Builder& setEveryDayScheduler(const std::shared_ptr<EveryDayScheduler>& everyDayScheduler);
	Builder& setHotWeatherScheduler(const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler);
	Builder& setTemperatureDependentScheduler(const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler);
	Builder& setWeeklyScheduler(const std::shared_ptr<WeeklyScheduler>& weeklyScheduler);
	Builder& setRunTimeContainer(const std::shared_ptr<RunTimeContainer>& runTimeContainer);
	Builder& setStartTimeContainer(const std::shared_ptr<StartTimeContainer>& startTimeContainer);
	Builder& setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory);

	ProgramPtr build();
};


class ProgramImplFactory::Builder {
	std::shared_ptr<EveryDaySchedulerFactory> everyDaySchedulerFactory;
	std::shared_ptr<HotWeatherSchedulerFactory> hotWeatherSchedulerFactory;
	std::shared_ptr<TemperatureDependentSchedulerFactory> temperatureDependentSchedulerFactory;
	std::shared_ptr<WeeklySchedulerFactory> weeklySchedulerFactory;
	std::shared_ptr<RunTimeContainerFactory> runTimeContainerFactory;
	std::shared_ptr<StartTimeContainerFactory> startTimeContainerFactory;
	std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	Builder();

	Builder& setEveryDaySchedulerFactory(const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory);
	Builder& setHotWeatherSchedulerFactory(const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory);
	Builder& setTemperatureDependentSchedulerFactory(const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory);
	Builder& setWeeklySchedulerFactory(const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory);
	Builder& setRunTimeContainerFactory(const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory);
	Builder& setStartTimeContainerFactory(const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory);
	Builder& setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory);

	std::shared_ptr<ProgramFactory> build();
};
