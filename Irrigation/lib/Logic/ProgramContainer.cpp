#include "ProgramContainer.h"
#include "Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"

using namespace std;


ProgramContainer::ProgramContainer() {
}

ProgramContainer::~ProgramContainer() {
	for (auto it = container.begin(); it != container.end(); ++it) {
		delete it->second;
	}
}

ProgramContainer::container_type::iterator ProgramContainer::find(const key_type& key) {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw InvalidProgramIdException(key);
}

ProgramContainer::container_type::const_iterator ProgramContainer::find(const key_type& key) const {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw InvalidProgramIdException(key);
}

const ProgramContainer::mapped_type& ProgramContainer::at(const key_type& key) const {
	return find(key)->second;
}

ProgramContainer::mapped_type& ProgramContainer::at(const key_type& key) {
	return find(key)->second;
}

ProgramContainer::value_type& ProgramContainer::insert(const key_type& key, const mapped_type& value) {

	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			delete value;
			throw ProgramIdExist(key);
		}
	}

	container.push_back(make_pair(key, value));
	LOGGER.info("Program %u added", key);
	return container.back();
}

void ProgramContainer::erase(const key_type& key) {
	auto it = find(key);
	delete it->second;
	container.erase(it);
	LOGGER.info("Program %u deleted", key);
}
