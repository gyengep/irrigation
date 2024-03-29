#pragma once
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "DTO/ProgramDto.h"
#include "Utils/DateTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/SchedulerContainer.h"
#include "Logic/Program.h"


class ProgramImpl : public Program {
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;

	const SchedulerContainerPtr schedulerContainer;
	const RunTimeContainerPtr runTimeContainer;
	const StartTimeContainerPtr startTimeContainer;

public:
	ProgramImpl(
		const SchedulerContainerPtr& schedulerContainer,
		const RunTimeContainerPtr& runTimeContainer,
		const StartTimeContainerPtr& startTimeContainer
	);

	ProgramImpl(
		bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
		const SchedulerContainerPtr& schedulerContainer,
		const RunTimeContainerPtr& runTimeContainer,
		const StartTimeContainerPtr& startTimeContainer
	);

	virtual ~ProgramImpl() = default;

	virtual void setEnabled(bool enabled) override						{ this->enabled = enabled; }
	virtual void setName(const std::string& name) override				{ this->name = name; }
	virtual void setAdjustment(unsigned adjustment) override			{ this->adjustment = adjustment; }
	virtual void setSchedulerType(SchedulerType schedulerType) override { this->schedulerType = schedulerType; }

	virtual bool isEnabled() const override								{ return enabled; }
	virtual const std::string& getName() const override					{ return name; }
	virtual unsigned getAdjustment() const override						{ return adjustment; }
	SchedulerType getSchedulerType() const override 					{ return schedulerType; }

	virtual std::unique_ptr<Scheduler::Result> isScheduled(const LocalDateTime& localDateTime) override;

	virtual const SchedulerContainer& getSchedulerContainer() const 	{ return *schedulerContainer; }
	virtual const RunTimeContainer& getRunTimeContainer() const 		{ return *runTimeContainer; }
	virtual const StartTimeContainer& getStartTimeContainer() const 	{ return *startTimeContainer; }

	virtual SchedulerContainer& getSchedulerContainer() 				{ return *schedulerContainer; }
	virtual RunTimeContainer& getRunTimeContainer() 					{ return *runTimeContainer; }
	virtual StartTimeContainer& getStartTimeContainer() 				{ return *startTimeContainer; }

	virtual ProgramDto toProgramDto() const override;
	virtual void updateFromProgramDto(const ProgramDto& programDto) override;

	virtual std::string toString() const override;

	virtual nlohmann::json saveTo() const override;
	virtual void loadFrom(const nlohmann::json& values) override;
};

///////////////////////////////////////////////////////////////////////////////

class ProgramImplFactory : public ProgramFactory {
	const SchedulerContainerFactoryPtr schedulerContainerFactory;
	const RunTimeContainerFactoryPtr runTimeContainerFactory;
	const StartTimeContainerFactoryPtr startTimeContainerFactory;

public:
	ProgramImplFactory(
		const SchedulerContainerFactoryPtr& schedulerContainerFactory,
		const RunTimeContainerFactoryPtr& runTimeContainerFactory,
		const StartTimeContainerFactoryPtr& startTimeContainerFactory
	);

	virtual ~ProgramImplFactory() = default;
	virtual ProgramPtr create() const override;
};
