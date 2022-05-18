#include "StartTimeContainerImpl.h"
#include <algorithm>
#include <sstream>
#include "Exceptions/Exceptions.h"

///////////////////////////////////////////////////////////////////////////////

StartTimeContainerPtr StartTimeContainerImplFactory::create() const {
	return std::make_shared<StartTimeContainerImpl>();
}

///////////////////////////////////////////////////////////////////////////////

StartTimeContainerImpl::StartTimeContainerImpl(std::initializer_list<StartTimeContainer::value_type> initializer) :
	container(initializer)
{
}

StartTimeContainerImpl::const_mapped_type StartTimeContainerImpl::at(const key_type& key) const {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("StartTime[" + key.toString() + "] does not exist");
	}

	return it->second;
}

StartTimeContainer::mapped_type StartTimeContainerImpl::at(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("StartTime[" + key.toString() + "] does not exist");
	}

	return it->second;
}

void StartTimeContainerImpl::erase(const key_type& key) {
	auto it = find_if(container.begin(), container.end(), findKey(key));

	if (container.end() == it) {
		throw NoSuchElementException("StartTime[" + key.toString() + "] does not exist");
	}

	container.erase(it);
}

StartTimeContainerImpl::value_type& StartTimeContainerImpl::insert(const key_type& key, const mapped_type& value) {
	if (container.end() != find_if(container.begin(), container.end(), findKey(key))) {
		throw AlreadyExistException("StartTime[" + key.toString() + "] already exists");
	}

	container.emplace_back(make_pair(key, value));
	return container.back();
}

void StartTimeContainerImpl::sort() {
	auto comp = [](const value_type& a, const value_type& b) {
		return a.second->operator <(*b.second);
	};

	container.sort(comp);
}

std::list<StartTimeDTO> StartTimeContainerImpl::toStartTimeDtoList() const {
	std::list<StartTimeDTO> startTimeDtoList;
	for (const value_type& value : container) {
		startTimeDtoList.push_back(value.second->toStartTimeDto().setId(value.first.getValue()));
	}
	return startTimeDtoList;
}

void StartTimeContainerImpl::updateFromStartTimeDtoList(const std::shared_ptr<StartTimeFactory>& startTimeFactory, const std::list<StartTimeDTO>& startTimeDtoList) {
	container.clear();
	for (const StartTimeDTO& startTimeDTO : startTimeDtoList) {
		std::unique_ptr<IdType> id;
		if (startTimeDTO.hasId()) {
			id.reset(new IdType(startTimeDTO.getId()));
		} else {
			id.reset(new IdType());
		}

		StartTimePtr startTime = startTimeFactory->create();
		startTime->updateFromStartTimeDto(startTimeDTO);
		insert(IdType(*id), startTime);
	}
}

std::string StartTimeContainerImpl::toString() const {
	std::ostringstream oss;

	oss << "[";
	for (auto it = begin(); it != end(); ++it) {
		if (it != begin()) {
			oss << ", ";
		}
		oss << "{" << it->first.toString() << ", " << it->second->toShortString() << "}";
	}
	oss << "]";

	return oss.str();
}
