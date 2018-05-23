#include "ProgramContainer.h"
#include "Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Utils/UniqueID.h"

using namespace std;


LockedProgram::LockedProgram(Program* program, std::mutex* mutex) :
	program(program),
	mutex(mutex)
{
	mutex->lock();
}

LockedProgram::~LockedProgram() {
	mutex->unlock();
}

Program* LockedProgram::operator-> () {
	return program;
}

const Program* LockedProgram::operator-> () const {
	return program;
}

const Program* LockedProgram::get() const {
	return program;
}

Program* LockedProgram::get() {
	return program;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ProgramContainer::InnerValueType::InnerValueType(const IdType id, Program* program) :
	id(id),
	program(program),
	mutex(new std::mutex)
{
}

ProgramContainer::InnerValueType::~InnerValueType() {
	mutex->lock();
	mutex->unlock();
	delete mutex;
	delete program;
}

ProgramContainer::ProgramContainer() {
}

ProgramContainer::~ProgramContainer() {
	lock_guard<std::mutex> lock(mutex);

	for (auto it = container.begin(); container.end() != it; ++it) {
		const InnerValueType* value = *it;
		delete value;
	}

	container.clear();
}

ProgramContainer::container_type::iterator ProgramContainer::find(const key_type& key) {
	for (auto it = container.begin(); container.end() != it; ++it) {
		const InnerValueType* value = *it;
		if (value->id == key) {
			return it;
		}
	}

	throw InvalidProgramIdException(key);
}

ProgramContainer::container_type::const_iterator ProgramContainer::find(const key_type& key) const {
	for (auto it = container.begin(); container.end() != it; ++it) {
		const InnerValueType* value = *it;
		if (value->id == key) {
			return it;
		}
	}

	throw InvalidProgramIdException(key);
}

size_t ProgramContainer::size() const {
	lock_guard<std::mutex> lock(mutex);
	return container.size();
}

const LockedProgramPtr ProgramContainer::at(const key_type& key) const {
	lock_guard<std::mutex> lock(mutex);

	const InnerValueType* value = *find(key);
	return LockedProgramPtr(new LockedProgram(value->program, value->mutex));
}

LockedProgramPtr ProgramContainer::at(const key_type& key) {
	lock_guard<std::mutex> lock(mutex);

	const InnerValueType* value = *find(key);
	return LockedProgramPtr(new LockedProgram(value->program, value->mutex));
}

void ProgramContainer::iterate(callback_type func) const {
	lock_guard<std::mutex> lock(mutex);

	for (auto it = container.begin(); it != container.end(); ++it) {
		const InnerValueType* value = *it;
		if (!func(value->id, LockedProgramPtr(new LockedProgram(value->program, value->mutex)))) {
			return;
		}
	}
}

void ProgramContainer::insert(const key_type& key, const mapped_type& value) {
	lock_guard<std::mutex> lock(mutex);

	for (auto it = container.begin(); it != container.end(); ++it) {
		const InnerValueType* value = *it;
		if (value->id == key) {
			throw ProgramIdExist(key);
		}
	}

	InnerValueType* innerValue = new InnerValueType(key, value);
	container.push_back(innerValue);
	LOGGER.info("Program %u added", key);
}

void ProgramContainer::erase(const key_type& key) {
	lock_guard<std::mutex> lock(mutex);

	auto it = find(key);
	delete *it;
	container.erase(it);
	LOGGER.info("Program %u deleted", key);
}
