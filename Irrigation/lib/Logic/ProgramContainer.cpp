#include "ProgramContainer.h"
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"

using namespace std;


ProgramContainer::ProgramContainer() {
}

ProgramContainer::~ProgramContainer() {
}

ProgramContainer::container_type::const_iterator ProgramContainer::find(const key_type& key) const {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw NoSuchElementException("Program with id " + to_string(key) + " not found");
}

const ProgramContainer::mapped_type::element_type* ProgramContainer::at(const key_type& key) const {
	return find(key)->second.get();
}

ProgramContainer::mapped_type::element_type* ProgramContainer::at(const key_type& key) {
	return find(key)->second.get();
}

ProgramContainer::value_type& ProgramContainer::insert(const key_type& key, mapped_type::element_type* value) {
	unique_ptr<Program> program(value);

	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			throw AlreadyExistException("Program with id " + to_string(key) + " is already exist");
		}
	}

	container.push_back(make_pair(key, move(program)));
	LOGGER.debug("Program[%s] added: %s",
		to_string(key).c_str(),
		to_string(*container.back().second.get()).c_str()
		);
	return container.back();
}

void ProgramContainer::erase(const key_type& key) {
	auto it = find(key);
	LOGGER.debug("Program[%s] deleted", to_string(key).c_str());
	container.erase(it);
}
