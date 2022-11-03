#pragma once
#include "ProgramImpl.h"


class ProgramImpl::Builder {
	bool enabled;
	std::string name;
	unsigned adjustment;
	SchedulerType schedulerType;
	std::shared_ptr<SchedulerContainer> schedulerContainer;
	std::shared_ptr<RunTimeContainer> runTimeContainer;
	std::shared_ptr<StartTimeContainer> startTimeContainer;
	std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	Builder();

	Builder& setEnabled(bool enabled);
	Builder& setName(const std::string& name);
	Builder& setAdjustment(unsigned adjustment);
	Builder& setSchedulerType(SchedulerType schedulerType);
	Builder& setSchedulerContainer(const std::shared_ptr<SchedulerContainer>& schedulerContainer);
	Builder& setRunTimeContainer(const std::shared_ptr<RunTimeContainer>& runTimeContainer);
	Builder& setStartTimeContainer(const std::shared_ptr<StartTimeContainer>& startTimeContainer);
	Builder& setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory);

	ProgramPtr build();
};


class ProgramImplFactory::Builder {
	std::shared_ptr<SchedulerContainerFactory> schedulerContainerFactory;
	std::shared_ptr<RunTimeContainerFactory> runTimeContainerFactory;
	std::shared_ptr<StartTimeContainerFactory> startTimeContainerFactory;
	std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	Builder();

	Builder& setSchedulerContainerFactory(const std::shared_ptr<SchedulerContainerFactory>& schedulerContainerFactory);
	Builder& setRunTimeContainerFactory(const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory);
	Builder& setStartTimeContainerFactory(const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory);
	Builder& setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory);

	std::shared_ptr<ProgramFactory> build();
};
