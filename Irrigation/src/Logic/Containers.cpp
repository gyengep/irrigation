#include "Common.h"
#include "Containers.h"
#include "Program.h"


static std::atomic<IdType> nextId(0);


////////////////////////////////////////////////////////////////
// RUN TIME


RunTimeContainer::RunTimeContainer() {
	for (size_t i = 0; i < container.size(); i++) {
		container[i] = std::make_pair(i, 0);
	}
}

RunTimeContainer::~RunTimeContainer() {
}

RunTimeContainer::value_type& RunTimeContainer::at(IdType id) {
	if (container.size() <= id) {
		throw InvalidRunTimeIdException();
	}

	return container[id].second;
}

const RunTimeContainer::value_type& RunTimeContainer::at(IdType id) const {
	return const_cast<RunTimeContainer*>(this)->at(id);
}

////////////////////////////////////////////////////////////////
// START TIME

StartTimeContainer::StartTimeContainer() {
}

StartTimeContainer::~StartTimeContainer() {
}

StartTimeContainer::ContainerType::iterator StartTimeContainer::find(IdType id) {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == id) {
			return it;
		}
	}

	throw InvalidStartTimeIdException();
}

void StartTimeContainer::insert(IdType id, const StartTimeContainer::value_type& value) {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (value < it->second) {
			container.insert(it, std::make_pair(id, value));
			return;
		}
	}

	container.push_back(std::make_pair(id, value));
}

const StartTimeContainer::value_type& StartTimeContainer::at(IdType id) const {
	return const_cast<StartTimeContainer*>(this)->find(id)->second;
}

IdType StartTimeContainer::insert(const value_type& newItem) {
	IdType startTimeId = nextId++;
	insert(startTimeId, newItem);
	return startTimeId;
}

void StartTimeContainer::modify(IdType id, const value_type& newItem) {
	ContainerType::iterator it = find(id);
	container.erase(it);
	insert(id, newItem);
}

void StartTimeContainer::erase(IdType id) {
	ContainerType::iterator it = find(id);
	container.erase(it);
}

/////////////////////////////////////////////////////
// Program

ProgramContainer::ProgramContainer() {
}

ProgramContainer::~ProgramContainer() {
	std::lock_guard<std::mutex> lock(mutex);

	for (auto programIt = programContainer.begin(); programContainer.end() != programIt; ++programIt) {
		IdType id = programIt->first;
		auto mutexIt = findMutex(id);

		std::mutex* programMutex = mutexIt->second;
		Program* program = programIt->second;

		programMutex->lock();
		programMutex->unlock();

		delete program;
		delete programMutex;
	}

	mutexContainer.clear();
	programContainer.clear();
}

ProgramContainer::ProgramContainerType::iterator ProgramContainer::findProgram(IdType id) {
	ProgramContainerType::iterator it;

	for (it = programContainer.begin(); it != programContainer.end(); ++it) {
		if (it->first == id) {
			return it;
		}
	}

	throw InvalidProgramIdException();
}

ProgramContainer::MutexContainerType::iterator ProgramContainer::findMutex(IdType id) {
	MutexContainerType::iterator it = mutexContainer.find(id);

	if (mutexContainer.end() != it) {
		return it;
	}

	throw InvalidProgramIdException();
}

size_t ProgramContainer::size() const {
	std::lock_guard<std::mutex> lock(mutex);
	return programContainer.size();
}

void ProgramContainer::iterate(CallbackType f) const {
	std::lock_guard<std::mutex> lock(mutex);

	for (auto programIt = programContainer.begin(); programIt != programContainer.end(); ++programIt) {
		auto mutexesIt = mutexContainer.find(programIt->first);
		if (mutexContainer.end() == mutexesIt) {
			throw std::runtime_error("Internal error! Mutex not found");
		}

		f(programIt->first, LockedProgram(programIt->second, mutexesIt->second));
	}
}

const LockedProgram ProgramContainer::at(IdType id) const {
	std::lock_guard<std::mutex> lock(mutex);

	auto mutexIt = const_cast<ProgramContainer*>(this)->findMutex(id);
	auto programIt = const_cast<ProgramContainer*>(this)->findProgram(id);
	return LockedProgram(programIt->second, mutexIt->second);
}

IdType ProgramContainer::insert(const value_type& newItem) {
	std::lock_guard<std::mutex> lock(mutex);

	IdType programId = nextId++;
	mutexContainer.insert(std::make_pair(programId, new std::mutex()));
	programContainer.push_back(std::make_pair(programId, newItem));
	return programId;
}


void ProgramContainer::erase(IdType id) {
	std::lock_guard<std::mutex> lock(mutex);

	auto mutexIt = findMutex(id);
	auto programIt = findProgram(id);

	std::mutex* programMutex = mutexIt->second;
	Program* program = programIt->second;

	programMutex->lock();
	mutexContainer.erase(mutexIt);
	programContainer.erase(programIt);
	programMutex->unlock();

	delete program;
	delete programMutex;
}

void ProgramContainer::move(IdType id, size_t newPosition) {
	std::lock_guard<std::mutex> lock(mutex);

	if (programContainer.size() <= newPosition) {
		throw std::out_of_range("Invalid position");
	}

	auto programIt = findProgram(id);
	Program* value = programIt->second;

	programContainer.erase(programIt);

	auto it = programContainer.begin();
	for (size_t count = 0; count < newPosition; ++count) {
		++it;
	}

	programContainer.insert(it, std::make_pair(id, value));
}
