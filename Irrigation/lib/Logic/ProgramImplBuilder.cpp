#include "ProgramImplBuilder.h"
#include "SchedulerContainerImpl.h"
#include "SchedulerContainerImplBuilder.h"
#include "RunTimeContainerImpl.h"
#include "StartTimeContainerImpl.h"
#include "Temperature/TemperatureHandler.h"


ProgramImpl::Builder::Builder() :
	enabled(true),
	name(),
	adjustment(100),
	schedulerType(SchedulerType::WEEKLY)
{
}

ProgramImpl::Builder& ProgramImpl::Builder::setEnabled(bool enabled) {
	this->enabled = enabled;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setName(const std::string& name) {
	this->name = name;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setSchedulerType(SchedulerType schedulerType) {
	this->schedulerType = schedulerType;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setSchedulerContainer(const std::shared_ptr<SchedulerContainer>& schedulerContainer) {
	this->schedulerContainer = schedulerContainer;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setRunTimeContainer(const std::shared_ptr<RunTimeContainer>& runTimeContainer) {
	this->runTimeContainer = runTimeContainer;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setStartTimeContainer(const std::shared_ptr<StartTimeContainer>& startTimeContainer) {
	this->startTimeContainer = startTimeContainer;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory) {
	this->startTimeFactory = startTimeFactory;
	return *this;
}

ProgramPtr ProgramImpl::Builder::build() {

	if (nullptr == schedulerContainer) {
		schedulerContainer = SchedulerContainerImpl::Builder().build();
	}

	if (nullptr == runTimeContainer) {
		runTimeContainer = std::make_shared<RunTimeContainerImpl>(
				std::make_shared<RunTimeFactory>()
			);
	}

	if (nullptr == startTimeContainer) {
		startTimeContainer = std::make_shared<StartTimeContainerImpl>();
	}

	if (nullptr == startTimeFactory) {
		startTimeFactory = std::make_shared<StartTimeFactory>();
	}

	return std::make_shared<ProgramImpl>(
			enabled,
			name,
			adjustment,
			schedulerType,
			schedulerContainer,
			runTimeContainer,
			startTimeContainer,
			startTimeFactory
		);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImplFactory::Builder::Builder() {
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setSchedulerContainerFactory(const std::shared_ptr<SchedulerContainerFactory>& schedulerContainerFactory) {
	this->schedulerContainerFactory = schedulerContainerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setRunTimeContainerFactory(const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory) {
	this->runTimeContainerFactory = runTimeContainerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setStartTimeContainerFactory(const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory) {
	this->startTimeContainerFactory = startTimeContainerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory) {
	this->startTimeFactory = startTimeFactory;
	return *this;
}

std::shared_ptr<ProgramFactory> ProgramImplFactory::Builder::build() {
	if (nullptr == schedulerContainerFactory) {
		schedulerContainerFactory = SchedulerContainerImplFactory::Builder().build();
	}

	if (nullptr == runTimeContainerFactory) {
		runTimeContainerFactory = std::make_shared<RunTimeContainerImplFactory>(
				std::make_shared<RunTimeFactory>()
			);
	}

	if (nullptr == startTimeContainerFactory) {
		startTimeContainerFactory = std::make_shared<StartTimeContainerImplFactory>();
	}

	if (nullptr == startTimeFactory) {
		startTimeFactory = std::make_shared<StartTimeFactory>();
	}

	return std::make_shared<ProgramImplFactory>(
			schedulerContainerFactory,
			runTimeContainerFactory,
			startTimeContainerFactory,
			startTimeFactory
		);
}
