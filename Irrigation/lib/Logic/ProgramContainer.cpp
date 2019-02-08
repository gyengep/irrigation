#include "ProgramContainer.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Logic/Program.h"

using namespace std;


ProgramContainer::ProgramContainer(const ProgramContainer& other) {
	for (const auto& value : other.container) {
		container.push_back(make_pair(value.first, shared_ptr<Program>(new Program(*value.second))));
	}
}

ProgramContainer::ProgramContainer(initializer_list<ProgramContainer::value_type> initializer) {
	for (const auto& value : initializer) {
		insert(value.first, value.second);
	}
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
		throw NoSuchElementException("Program[" + to_string(key) + "] not found");
	}

	return it->second;
}

void ProgramContainer::erase(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + to_string(key) + "] not found");
	}

	container.erase(it);
}

ProgramContainer::value_type& ProgramContainer::insert(const key_type& key, const mapped_type& value) {
	if (container.end() != find_if(container.begin(), container.end(), findKey(key))) {
		throw AlreadyExistException("Program[" + to_string(key) + "] is already exist");
	}

	container.push_back(make_pair(key, value));
	return container.back();
}

list<ProgramDTO> ProgramContainer::toProgramDtoList() const {
	list<ProgramDTO> programDtos;
	for (const value_type& value : container) {
		programDtos.push_back(value.second->toProgramDto().setId(value.first));
	}
	return programDtos;
}

void ProgramContainer::updateFromProgramDtoList(const list<ProgramDTO>& dtoList) {
	container.clear();
	for (const ProgramDTO& dto : dtoList) {
		unique_ptr<IdType> id;
		if (dto.hasId()) {
			id.reset(new IdType(dto.getId()));
		} else {
			id.reset(new IdType());
		}

		shared_ptr<Program> program = Program::Builder().build();
		program->updateFromProgramDto(dto);
		insert(IdType(*id), program);
	}
}

string to_string(const ProgramContainer& programContainer) {
	ostringstream oss;
	oss << programContainer;
	return oss.str();
}

ostream& operator<<(ostream& os, const ProgramContainer& programContainer) {
	os << "[";
	for (auto it = programContainer.begin(); it != programContainer.end(); ++it) {
		if (it != programContainer.begin()) {
			os << ", ";
		}
		os << "{" << to_string(it->first) << ", " << to_string(*it->second) << "}";
	}
	os << "]";
	return os;
}
