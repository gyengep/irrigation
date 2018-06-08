#pragma once
#include <list>
#include <memory>
#include <string>
#include "EqualsPtr.h"
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "SchedulersDTO.h"



class ProgramDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const std::string> name;
	std::unique_ptr<const std::string> schedulerType;
	std::unique_ptr<const SpecifiedSchedulerDTO> specifiedScheduler;
	std::unique_ptr<const std::list<RunTimeDTO>> runTimes;
	std::unique_ptr<const std::list<StartTimeDTO>> startTimes;

public:
	ProgramDTO() {
	}

	ProgramDTO(const std::string& name, const std::string& schedulerType,
			const SpecifiedSchedulerDTO& specifiedScheduler,
			const std::list<RunTimeDTO>* runTimes,
			const std::list<StartTimeDTO>* startTimes) {

		setName(name.c_str());
		setSchedulerType(schedulerType.c_str());
		setSpecifiedScheduler(specifiedScheduler);

		if (runTimes != nullptr) {
			setRunTimes(runTimes);
		}

		if (startTimes != nullptr) {
			setStartTimes(startTimes);
		}
	}

	ProgramDTO(const ProgramDTO& other) {
		if (other.hasId()) {
			setId(other.getId());
		}

		if (other.hasName()) {
			setName(other.getName().c_str());
		}

		if (other.hasSchedulerType()) {
			setSchedulerType(other.getSchedulerType().c_str());
		}

		if (other.hasSpecifiedScheduler()) {
			setSpecifiedScheduler(other.getSpecifiedScheduler());
		}

		if (other.hasRunTimes()) {
			setRunTimes(new std::list<RunTimeDTO>(other.getRunTimes()));
		}

		if (other.hasStartTimes()) {
			setStartTimes(new std::list<StartTimeDTO>(other.getStartTimes()));
		}
	}

	bool operator== (const ProgramDTO& other) const {
		return (equalsPtr(id.get(), other.id.get()) &&
				equalsPtr(name.get(), other.name.get()) &&
				equalsPtr(schedulerType.get(), other.schedulerType.get()) &&
				equalsPtr(specifiedScheduler.get(), other.specifiedScheduler.get()) &&
				equalsPtr(runTimes.get(), other.runTimes.get()) &&
				equalsPtr(startTimes.get(), other.startTimes.get()));
	}

	bool hasId() const {
		return (id.get() != nullptr);
	}

	unsigned getId() const {
		if (!hasId()) {
			throw std::logic_error("ProgramDTO::getId()");
		}

		return *id.get();
	}

	ProgramDTO& setId(unsigned id) {
		this->id.reset(new unsigned(id));
		return *this;
	}

	bool hasName() const {
		return (name.get() != nullptr);
	}

	const std::string& getName() const {
		if (!hasName()) {
			throw std::logic_error("ProgramDTO::getName()");
		}

		return *name.get();
	}

	ProgramDTO& setName(const char* name) {
		this->name.reset(new std::string(name));
		return *this;
	}

	bool hasSchedulerType() const {
		return (schedulerType.get() != nullptr);
	}

	const std::string& getSchedulerType() const {
		if (!hasSchedulerType()) {
			throw std::logic_error("ProgramDTO::getSchedulerType()");
		}

		return *schedulerType.get();
	}

	ProgramDTO& setSchedulerType(const char* schedulerType) {
		this->schedulerType.reset(new std::string(schedulerType));
		return *this;
	}

	bool hasSpecifiedScheduler() const {
		return (specifiedScheduler.get() != nullptr);
	}

	const SpecifiedSchedulerDTO& getSpecifiedScheduler() const {
		if (!hasSpecifiedScheduler()) {
			throw std::logic_error("ProgramDTO::getSpecifiedScheduler()");
		}

		return *specifiedScheduler.get();
	}

	ProgramDTO& setSpecifiedScheduler(const SpecifiedSchedulerDTO& specifiedScheduler) {
		this->specifiedScheduler.reset(new SpecifiedSchedulerDTO(specifiedScheduler));
		return *this;
	}

	bool hasRunTimes() const {
		return (runTimes.get() != nullptr);
	}

	const std::list<RunTimeDTO>& getRunTimes() const {
		if (!hasRunTimes()) {
			throw std::logic_error("ProgramDTO::getRunTimes()");
		}

		return *runTimes.get();
	}

	ProgramDTO& setRunTimes(const std::list<RunTimeDTO>* runTimes) {
		this->runTimes.reset(runTimes);
		return *this;
	}

	bool hasStartTimes() const {
		return (startTimes.get() != nullptr);
	}

	const std::list<StartTimeDTO>& getStartTimes() const {
		if (!hasStartTimes()) {
			throw std::logic_error("ProgramDTO::getStartTimes()");
		}

		return *startTimes.get();
	}

	ProgramDTO& setStartTimes(const std::list<StartTimeDTO>* startTimes) {
		this->startTimes.reset(startTimes);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program) {
	    os << "ProgramDTO{";
	    PRINT_PTR(os, "id", program.id.get());
	    os << ", ";
	    PRINT_STR(os, "name", program.name.get());
	    os << ", ";
	    PRINT_STR(os, "schedulerType", program.schedulerType.get());
	    os << ", ";
	    PRINT_PTR(os, "specifiedScheduler", program.specifiedScheduler.get());
	    os << ", ";
	    PRINT_PTR(os, "runTimes", program.runTimes.get());
	    os << ", ";
	    PRINT_PTR(os, "startTimes", program.startTimes.get());
		os << "}";

	    return os;
	}
};
