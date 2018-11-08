#include "ProgramDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


ProgramDTO::ProgramDTO(const ProgramDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasName()) {
		setName(other.getName());
	}

	if (other.hasSchedulerType()) {
		setSchedulerType(other.getSchedulerType());
	}

	if (other.hasSpecifiedScheduler()) {
		setSpecifiedScheduler(SpecifiedSchedulerDTO(other.getSpecifiedScheduler()));
	}

	if (other.hasRunTimes()) {
		setRunTimes(list<RunTimeDTO>(other.getRunTimes()));
	}

	if (other.hasStartTimes()) {
		setStartTimes(list<StartTimeDTO>(other.getStartTimes()));
	}
}

ProgramDTO::ProgramDTO(const string& name, const string& schedulerType,
		SpecifiedSchedulerDTO&& specifiedScheduler,
		list<RunTimeDTO>&& runTimes,
		list<StartTimeDTO>&& startTimes) {

	setName(name);
	setSchedulerType(schedulerType);
	setSpecifiedScheduler(move(specifiedScheduler));
	setRunTimes(move(runTimes));
	setStartTimes(move(startTimes));
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
		throw logic_error("ProgramDTO::getId(): !hasId()");
	}

	return *id.get();
}

const string& ProgramDTO::getName() const {
	if (!hasName()) {
		throw logic_error("ProgramDTO::getName(): !hasName()");
	}

	return *name.get();
}

const string& ProgramDTO::getSchedulerType() const {
	if (!hasSchedulerType()) {
		throw logic_error("ProgramDTO::getSchedulerType(): !hasSchedulerType()");
	}

	return *schedulerType.get();
}

const SpecifiedSchedulerDTO& ProgramDTO::getSpecifiedScheduler() const {
	if (!hasSpecifiedScheduler()) {
		throw logic_error("ProgramDTO::getSpecifiedScheduler(): !hasSpecifiedScheduler()");
	}

	return *specifiedScheduler.get();
}

const list<RunTimeDTO>& ProgramDTO::getRunTimes() const {
	if (!hasRunTimes()) {
		throw logic_error("ProgramDTO::getRunTimes(): !hasRunTimes()");
	}

	return *runTimes.get();
}

const list<StartTimeDTO>& ProgramDTO::getStartTimes() const {
	if (!hasStartTimes()) {
		throw logic_error("ProgramDTO::getStartTimes(): !hasStartTimes()");
	}

	return *startTimes.get();
}

ProgramDTO& ProgramDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

ProgramDTO& ProgramDTO::setName(const string& name) {
	this->name.reset(new string(name));
	return *this;
}

ProgramDTO& ProgramDTO::setSchedulerType(const string& schedulerType) {
	this->schedulerType.reset(new string(schedulerType));
	return *this;
}

ProgramDTO& ProgramDTO::setSpecifiedScheduler(SpecifiedSchedulerDTO&& specifiedScheduler) {
	this->specifiedScheduler.reset(new SpecifiedSchedulerDTO(move(specifiedScheduler)));
	return *this;
}

ProgramDTO& ProgramDTO::setRunTimes(list<RunTimeDTO>&& runTimes) {
	this->runTimes.reset(new list<RunTimeDTO>(move(runTimes)));
	return *this;
}

ProgramDTO& ProgramDTO::setStartTimes(list<StartTimeDTO>&& startTimes) {
	this->startTimes.reset(new list<StartTimeDTO>(move(startTimes)));
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
