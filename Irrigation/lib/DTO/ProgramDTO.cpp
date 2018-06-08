#include "ProgramDTO.h"
#include "EqualsPtr.h"
#include <stdexcept>

using namespace std;


ProgramDTO::ProgramDTO() {
}

ProgramDTO::ProgramDTO(const string& name, const string& schedulerType,
		const SpecifiedSchedulerDTO& specifiedScheduler,
		const list<RunTimeDTO>* runTimes,
		const list<StartTimeDTO>* startTimes) {

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

ProgramDTO::ProgramDTO(const ProgramDTO& other) {
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
		setRunTimes(new list<RunTimeDTO>(other.getRunTimes()));
	}

	if (other.hasStartTimes()) {
		setStartTimes(new list<StartTimeDTO>(other.getStartTimes()));
	}
}

bool ProgramDTO::operator== (const ProgramDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(name.get(), other.name.get()) &&
			equalsPtr(schedulerType.get(), other.schedulerType.get()) &&
			equalsPtr(specifiedScheduler.get(), other.specifiedScheduler.get()) &&
			equalsPtr(runTimes.get(), other.runTimes.get()) &&
			equalsPtr(startTimes.get(), other.startTimes.get()));
}

bool ProgramDTO::hasId() const {
	return (id.get() != nullptr);
}

bool ProgramDTO::hasName() const {
	return (name.get() != nullptr);
}

bool ProgramDTO::hasSchedulerType() const {
	return (schedulerType.get() != nullptr);
}

bool ProgramDTO::hasSpecifiedScheduler() const {
	return (specifiedScheduler.get() != nullptr);
}

bool ProgramDTO::hasRunTimes() const {
	return (runTimes.get() != nullptr);
}

bool ProgramDTO::hasStartTimes() const {
	return (startTimes.get() != nullptr);
}

unsigned ProgramDTO::getId() const {
	if (!hasId()) {
		throw logic_error("ProgramDTO::getId()");
	}

	return *id.get();
}

const string& ProgramDTO::getName() const {
	if (!hasName()) {
		throw logic_error("ProgramDTO::getName()");
	}

	return *name.get();
}

const string& ProgramDTO::getSchedulerType() const {
	if (!hasSchedulerType()) {
		throw logic_error("ProgramDTO::getSchedulerType()");
	}

	return *schedulerType.get();
}

const SpecifiedSchedulerDTO& ProgramDTO::getSpecifiedScheduler() const {
	if (!hasSpecifiedScheduler()) {
		throw logic_error("ProgramDTO::getSpecifiedScheduler()");
	}

	return *specifiedScheduler.get();
}

const list<RunTimeDTO>& ProgramDTO::getRunTimes() const {
	if (!hasRunTimes()) {
		throw logic_error("ProgramDTO::getRunTimes()");
	}

	return *runTimes.get();
}

const list<StartTimeDTO>& ProgramDTO::getStartTimes() const {
	if (!hasStartTimes()) {
		throw logic_error("ProgramDTO::getStartTimes()");
	}

	return *startTimes.get();
}

ProgramDTO& ProgramDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

ProgramDTO& ProgramDTO::setName(const char* name) {
	this->name.reset(new string(name));
	return *this;
}

ProgramDTO& ProgramDTO::setSchedulerType(const char* schedulerType) {
	this->schedulerType.reset(new string(schedulerType));
	return *this;
}

ProgramDTO& ProgramDTO::setSpecifiedScheduler(const SpecifiedSchedulerDTO& specifiedScheduler) {
	this->specifiedScheduler.reset(new SpecifiedSchedulerDTO(specifiedScheduler));
	return *this;
}

ProgramDTO& ProgramDTO::setRunTimes(const list<RunTimeDTO>* runTimes) {
	this->runTimes.reset(runTimes);
	return *this;
}

ProgramDTO& ProgramDTO::setStartTimes(const list<StartTimeDTO>* startTimes) {
	this->startTimes.reset(startTimes);
	return *this;
}

ostream& operator<<(ostream& os, const ProgramDTO& program) {
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
