#pragma once
#include <ctime>
#include <memory>
#include <string>
#include "json.hpp"
#include "DTO/ProgramDTO.h"
#include "Schedulers/Scheduler.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "RunTimeContainer.h"
#include "StartTimeContainer.h"
#include "Program.h"


class ProgramImpl : public Program {
public:
	class Builder;

private:
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;

	const std::shared_ptr<EveryDayScheduler> everyDayScheduler;
	const std::shared_ptr<HotWeatherScheduler> hotWeatherScheduler;
	const std::shared_ptr<TemperatureDependentScheduler> temperatureDependentScheduler;
	const std::shared_ptr<WeeklyScheduler> weeklyScheduler;
	const std::shared_ptr<RunTimeContainer> runTimeContainer;
	const std::shared_ptr<StartTimeContainer> startTimeContainer;
	const std::shared_ptr<StartTimeFactory> startTimeFactory;

	std::shared_ptr<Scheduler> currentScheduler;

public:
	ProgramImpl(
		const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
		const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
		const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
		const std::shared_ptr<WeeklyScheduler>& weeklyScheduler,
		const std::shared_ptr<RunTimeContainer>& runTimeContainer,
		const std::shared_ptr<StartTimeContainer>& startTimeContainer,
		const std::shared_ptr<StartTimeFactory>& startTimeFactory
	);

	ProgramImpl(
		bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
		const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
		const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
		const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
		const std::shared_ptr<WeeklyScheduler>& weeklyScheduler,
		const std::shared_ptr<RunTimeContainer>& runTimeContainer,
		const std::shared_ptr<StartTimeContainer>& startTimeContainer,
		const std::shared_ptr<StartTimeFactory>& startTimeFactory
	);

	ProgramImpl(ProgramImpl&&) = default;
	ProgramImpl(const ProgramImpl&) = delete;

	virtual ~ProgramImpl() = default;

	virtual void setEnabled(bool enabled) override						{ this->enabled = enabled; }
	virtual void setName(const std::string& name) override				{ this->name = name; }
	virtual void setAdjustment(unsigned adjustment) override			{ this->adjustment = adjustment; }
	virtual void setSchedulerType(SchedulerType schedulerType) override;

	virtual bool isEnabled() const override								{ return enabled; }
	virtual const std::string& getName() const override					{ return name; }
	virtual unsigned getAdjustment() const override						{ return adjustment; }
	SchedulerType getSchedulerType() const override;

	virtual std::unique_ptr<ScheduledResult> isScheduled(const std::time_t rawtime) override;
//	virtual const Scheduler& getCurrentScheduler() const { return *currentScheduler; }
	virtual Scheduler& getCurrentScheduler() override 					{ return *currentScheduler; }

	virtual const EveryDayScheduler& getEveryDayScheduler() const 		{ return *everyDayScheduler; }
	virtual const HotWeatherScheduler& getHotWeatherScheduler() const 	{ return *hotWeatherScheduler; }
	virtual const TemperatureDependentScheduler& getTemperatureDependentScheduler() const { return *temperatureDependentScheduler; }
	virtual const WeeklyScheduler& getWeeklyScheduler() const 			{ return *weeklyScheduler; }
	virtual const RunTimeContainer& getRunTimeContainer() const 		{ return *runTimeContainer; }
	virtual const StartTimeContainer& getStartTimeContainer() const 	{ return *startTimeContainer; }

	virtual EveryDayScheduler& getEveryDayScheduler() 					{ return *everyDayScheduler; }
	virtual HotWeatherScheduler& getHotWeatherScheduler() 				{ return *hotWeatherScheduler; }
	virtual TemperatureDependentScheduler& getTemperatureDependentScheduler() { return *temperatureDependentScheduler; }
	virtual WeeklyScheduler& getWeeklyScheduler() 						{ return *weeklyScheduler; }
	virtual RunTimeContainer& getRunTimeContainer() 					{ return *runTimeContainer; }
	virtual StartTimeContainer& getStartTimeContainer() 				{ return *startTimeContainer; }

	virtual std::pair<IdType, StartTimePtr> createStartTime(const StartTimeDTO& startTimeDto) override;

	virtual ProgramDTO toProgramDto() const override;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO) override;

	virtual std::string toString() const override;

	virtual nlohmann::json saveTo() const override;
	virtual void loadFrom(const nlohmann::json& values) override;
};

///////////////////////////////////////////////////////////////////////////////

class ProgramImplFactory : public ProgramFactory {
public:
	class Builder;

private:
	const std::shared_ptr<EveryDaySchedulerFactory> everyDaySchedulerFactory;
	const std::shared_ptr<HotWeatherSchedulerFactory> hotWeatherSchedulerFactory;
	const std::shared_ptr<TemperatureDependentSchedulerFactory> temperatureDependentSchedulerFactory;
	const std::shared_ptr<WeeklySchedulerFactory> weeklySchedulerFactory;
	const std::shared_ptr<RunTimeContainerFactory> runTimeContainerFactory;
	const std::shared_ptr<StartTimeContainerFactory> startTimeContainerFactory;
	const std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	ProgramImplFactory(
		const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory,
		const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory,
		const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory,
		const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory,
		const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory,
		const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory,
		const std::shared_ptr<StartTimeFactory>& startTimeFactory
	);

	virtual ~ProgramImplFactory() = default;
	virtual ProgramPtr create() const override;
};
