#pragma once
#include <list>
#include <memory>
#include <string>
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "SchedulersDTO.h"



class ProgramDTO {
	std::unique_ptr<const std::string> name;
	std::unique_ptr<const std::string> schedulerType;
	std::unique_ptr<const SpecifiedSchedulerDTO> specifiedScheduler;
	std::unique_ptr<const std::list<RunTimeDTO>> runTimes;
	std::unique_ptr<const std::list<StartTimeDTO>> startTimes;

public:
	ProgramDTO() {
	}

	ProgramDTO(const char* name, const char* schedulerType,
			const SpecifiedSchedulerDTO* specifiedScheduler,
			const std::list<RunTimeDTO>* runTimes,
			const std::list<StartTimeDTO>* startTimes) {
		if (name != nullptr) {
			setName(name);
		}

		if (schedulerType != nullptr) {
			setSchedulerType(schedulerType);
		}

		if (specifiedScheduler != nullptr) {
			setSpecifiedScheduler(specifiedScheduler);
		}

		if (runTimes != nullptr) {
			setRunTimes(runTimes);
		}

		if (startTimes != nullptr) {
			setStartTimes(startTimes);
		}
	}

	const char* getName() const {
		return name.get() ? name.get()->c_str() : NULL;
	}
	void setName(const char* name) {
		this->name.reset(new std::string(name));
	}

	const char* getSchedulerType() const {
		return schedulerType.get() ? schedulerType.get()->c_str() : NULL;
	}

	void setSchedulerType(const char* schedulerType) {
		this->schedulerType.reset(new std::string(schedulerType));
	}

	const SpecifiedSchedulerDTO* getSpecifiedScheduler() const {
		return specifiedScheduler.get();
	}

	void setSpecifiedScheduler(const SpecifiedSchedulerDTO* specifiedScheduler) {
		this->specifiedScheduler.reset(specifiedScheduler);
	}

	const std::list<RunTimeDTO>* getRunTimes() const {
		return runTimes.get();
	}

	void setRunTimes(const std::list<RunTimeDTO>* runTimes) {
		this->runTimes.reset(runTimes);
	}

	const std::list<StartTimeDTO>* getStartTimes() const {
		return startTimes.get();
	}

	void setStartTimes(const std::list<StartTimeDTO>* startTimes) {
		this->startTimes.reset(startTimes);
	}
};
