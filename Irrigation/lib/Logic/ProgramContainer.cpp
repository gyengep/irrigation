#include "ProgramContainer.h"
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"

using namespace std;


ProgramContainer::container_type::const_iterator ProgramContainer::find(const key_type& key) const {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == key) {
			return it;
		}
	}

	throw NoSuchElementException("Program[" + to_string(key) + "] not found");
}

const ProgramContainer::mapped_type& ProgramContainer::at(const key_type& key) const {
	return find(key)->second;
}

ProgramContainer::value_type& ProgramContainer::insert(const key_type& key, mapped_type&& value) {
	for (auto it = container.begin(); it != container.end(); ++it) {
		if (it->first == key) {
			throw AlreadyExistException("Program[" + to_string(key) + "] is already exist");
		}
	}

	container.push_back(make_pair(key, move(value)));
	LOGGER.debug("Program[%s] added: %s",
		to_string(key).c_str(),
		to_string(*container.back().second.get()).c_str()
		);
	return container.back();
}

void ProgramContainer::erase(const key_type& key) {
	container.erase(find(key));
	LOGGER.debug("Program[%s] deleted", to_string(key).c_str());
}
