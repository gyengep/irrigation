#include "ProgramContainerImpl.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"

///////////////////////////////////////////////////////////////////////////////

ProgramContainerPtr ProgramContainerImplFactory::create() const {
	return std::make_shared<ProgramContainerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

ProgramContainerImpl::ProgramContainerImpl(std::initializer_list<ProgramContainer::value_type> initializer) :
	container(initializer)
{
}

ProgramContainer::mapped_type ProgramContainerImpl::at(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + key.toString() + "] does not exist");
	}

	return it->second;
}

ProgramContainer::const_mapped_type ProgramContainerImpl::at(const key_type& key) const {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + key.toString() + "] does not exist");
	}

	return it->second;
}

void ProgramContainerImpl::erase(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("Program[" + key.toString() + "] does not exist");
	}

	container.erase(it);
}

ProgramContainerImpl::value_type& ProgramContainerImpl::insert(const key_type& key, const mapped_type& value) {
	if (container.end() != find_if(container.begin(), container.end(), findKey(key))) {
		throw AlreadyExistException("Program[" + key.toString() + "] already exists");
	}

	container.push_back(make_pair(key, value));
	return container.back();
}

std::list<ProgramDTO> ProgramContainerImpl::toProgramDtoList() const {
	std::list<ProgramDTO> programDtos;
	for (const value_type& value : container) {
		programDtos.push_back(value.second->toProgramDto().setId(value.first.getValue()));
	}
	return programDtos;
}

void ProgramContainerImpl::updateFromProgramDtoList(const std::shared_ptr<ProgramFactory>& programFactory, const std::list<ProgramDTO>& dtoList) {
	container.clear();
	for (const ProgramDTO& dto : dtoList) {
		std::unique_ptr<IdType> id;
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

std::string ProgramContainerImpl::toString() const {
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
