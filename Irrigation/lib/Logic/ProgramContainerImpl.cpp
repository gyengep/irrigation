#include "Impl/ProgramContainerImpl.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"

///////////////////////////////////////////////////////////////////////////////

ProgramContainerImplFactory::ProgramContainerImplFactory(const std::shared_ptr<ProgramFactory>& programFactory) :
	programFactory(programFactory)
{
}

ProgramContainerPtr ProgramContainerImplFactory::create() const {
	return std::make_shared<ProgramContainerImpl>(programFactory);
}

///////////////////////////////////////////////////////////////////////////////

ProgramContainerImpl::ProgramContainerImpl(const std::shared_ptr<ProgramFactory>& programFactory) :
	programFactory(programFactory)
{
}

ProgramContainerImpl::ProgramContainerImpl(const std::shared_ptr<ProgramFactory>& programFactory, std::initializer_list<ProgramContainer::value_type> initializer) :
	programFactory(programFactory),
	container(initializer)
{
}

ProgramContainerImpl::ProgramContainerImpl(std::initializer_list<ProgramContainer::value_type> initializer) :
	programFactory(nullptr),
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

void ProgramContainerImpl::updateFromProgramDtoList(const std::list<ProgramDTO>& programDtoList) {
	container.clear();

	for (const ProgramDTO& programDto : programDtoList) {
		if (programDto.hasId()) {
			createUpdateAndInsert(IdType(programDto.getId()), programDto);
		} else {
			createUpdateAndInsert(IdType(), programDto);
		}
	}
}

ProgramContainer::value_type& ProgramContainerImpl::createFromProgramDto(const ProgramDTO& programDto) {
	return createUpdateAndInsert(IdType(), programDto);
}

ProgramContainer::value_type& ProgramContainerImpl::createUpdateAndInsert(const IdType& id, const ProgramDTO& programDto) {
	auto program = programFactory->create();
	program->updateFromProgramDto(programDto);
	return insert(id, program);
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
