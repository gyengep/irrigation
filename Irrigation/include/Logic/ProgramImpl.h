#pragma once
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include "DTO/ProgramDTO.h"
#include "Utils/DateTime.h"
#include "RunTimeContainer.h"
#include "StartTimeContainer.h"
#include "SchedulerContainer.h"
#include "Program.h"


class ProgramImpl : public Program {
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;

	const std::shared_ptr<SchedulerContainer> schedulerContainer;
	const std::shared_ptr<RunTimeContainer> runTimeContainer;
	const std::shared_ptr<StartTimeContainer> startTimeContainer;

public:
	ProgramImpl(
		const std::shared_ptr<SchedulerContainer>& schedulerContainer,
		const std::shared_ptr<RunTimeContainer>& runTimeContainer,
		const std::shared_ptr<StartTimeContainer>& startTimeContainer
	);

	ProgramImpl(
		bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
		const std::shared_ptr<SchedulerContainer>& schedulerContainer,
		const std::shared_ptr<RunTimeContainer>& runTimeContainer,
		const std::shared_ptr<StartTimeContainer>& startTimeContainer
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

	virtual ProgramDTO toProgramDto() const override;
	virtual void updateFromProgramDto(const ProgramDTO& programDTO) override;

	virtual std::string toString() const override;

	virtual nlohmann::json saveTo() const override;
	virtual void loadFrom(const nlohmann::json& values) override;
};

///////////////////////////////////////////////////////////////////////////////

class ProgramImplFactory : public ProgramFactory {
	const std::shared_ptr<SchedulerContainerFactory> schedulerContainerFactory;
	const std::shared_ptr<RunTimeContainerFactory> runTimeContainerFactory;
	const std::shared_ptr<StartTimeContainerFactory> startTimeContainerFactory;

public:
	ProgramImplFactory(
		const std::shared_ptr<SchedulerContainerFactory>& schedulerContainerFactory,
		const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory,
		const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory
	);

	virtual ~ProgramImplFactory() = default;
	virtual ProgramPtr create() const override;
};
