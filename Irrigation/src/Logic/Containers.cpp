#include "common.h"
#include "Containers.h"

#include "Logic/Program.h"
#include "Utils/UniqueID.h"


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
	IdType startTimeId = UniqueID::getInstance().getNextId();
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

	for (auto it = programContainer.begin(); programContainer.end() != it; ++it) {
		it->second->mutex->lock();
		it->second->mutex->unlock();
	}

	programContainer.clear();
}

ProgramContainer::ProgramContainerType::iterator ProgramContainer::find(IdType id) {
	ProgramContainerType::iterator it;

	for (it = programContainer.begin(); it != programContainer.end(); ++it) {
		if (it->first == id) {
			return it;
		}
	}

	throw InvalidProgramIdException();
}

size_t ProgramContainer::size() const {
	std::lock_guard<std::mutex> lock(mutex);
	return programContainer.size();
}

void ProgramContainer::iterate(CallbackType f) const {
	std::lock_guard<std::mutex> lock(mutex);

	for (auto it = programContainer.begin(); it != programContainer.end(); ++it) {
		f(it->first, LockedProgram(it->second));
	}
}

const LockedProgram ProgramContainer::at(IdType id) const {
	std::lock_guard<std::mutex> lock(mutex);

	auto it = const_cast<ProgramContainer*>(this)->find(id);
	return LockedProgram(it->second);
}

IdType ProgramContainer::insert(const value_type& newItem) {
	std::lock_guard<std::mutex> lock(mutex);

	IdType programId = UniqueID::getInstance().getNextId();
	ProgramWithMutexPtr programWithMutexPtr(new ProgramWithMutex(newItem, new std::mutex()));
	programContainer.push_back(std::make_pair(programId, programWithMutexPtr));
	return programId;
}

void ProgramContainer::erase(IdType id) {
	std::lock_guard<std::mutex> lock(mutex);

	auto it = find(id);

	it->second->mutex->lock();
	it->second->mutex->unlock();

	programContainer.erase(it);
}

void ProgramContainer::move(IdType id, size_t newPosition) {
	std::lock_guard<std::mutex> lock(mutex);

	if (programContainer.size() <= newPosition) {
		throw std::out_of_range("Invalid position");
	}

	auto it = find(id);
	ProgramContainerType::value_type listItem = *it;
	programContainer.erase(it);

	it = programContainer.begin();
	for (size_t count = 0; count < newPosition; ++count) {
		++it;
	}

	programContainer.insert(it, listItem);
}
