#include "ProgramContainer.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Logic/Program.h"

using namespace std;

ProgramContainer::ProgramContainer(initializer_list<ProgramContainer::value_type> initializer) :
	container(initializer)
{
}

bool ProgramContainer::operator== (const ProgramContainer& other) const {
	if (container.size() != other.container.size()) {
		return false;
	}

	auto comp = [](const value_type& a, const value_type& b) {
		return (a.first == b.first) && (a.second->operator ==(*b.second));
	};

	return equal(container.begin(), container.end(), other.container.begin(), comp);
}

const ProgramContainer::mapped_type& ProgramContainer::at(const key_type& key) const {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + key.toString() + "] does not exist");
	}

	return it->second;
}

void ProgramContainer::erase(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + key.toString() + "] does not exist");
	}

	container.erase(it);
}

ProgramContainer::value_type& ProgramContainer::insert(const key_type& key, const mapped_type& value) {
	if (container.end() != find_if(container.begin(), container.end(), findKey(key))) {
		throw AlreadyExistException("Program[" + key.toString() + "] already exists");
	}

	container.push_back(make_pair(key, value));
	return container.back();
}

list<ProgramDTO> ProgramContainer::toProgramDtoList() const {
	list<ProgramDTO> programDtos;
	for (const value_type& value : container) {
		programDtos.push_back(value.second->toProgramDto().setId(value.first.getValue()));
	}
	return programDtos;
}

void ProgramContainer::updateFromProgramDtoList(const std::shared_ptr<ProgramFactory>& programFactory, const std::list<ProgramDTO>& dtoList) {
	container.clear();
	for (const ProgramDTO& dto : dtoList) {
		unique_ptr<IdType> id;
		if (dto.hasId()) {
			id.reset(new IdType(dto.getId()));
		} else {
			id.reset(new IdType());
		}

		ProgramPtr program = programFactory->create();
		program->updateFromProgramDto(dto);
		insert(IdType(*id), program);
	}
}

std::string ProgramContainer::toString() const {
	std::ostringstream oss;

	oss << "[";
	for (auto it = begin(); it != end(); ++it) {
		if (it != begin()) {
			oss << ", ";
		}
		oss << "{" << it->first.toString() << ", " << it->second->toString() << "}";
	}
	oss << "]";

	return oss.str();
}
