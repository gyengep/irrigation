#include "RunTimeContainerImpl.h"
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"

///////////////////////////////////////////////////////////////////////////////

RunTimeContainerImplFactory::RunTimeContainerImplFactory(const std::shared_ptr<RunTimeFactory>& runTimeFactory) :
	runTimeFactory(runTimeFactory)
{
}

RunTimeContainerPtr RunTimeContainerImplFactory::create() const {
	return std::make_shared<RunTimeContainerImpl>(runTimeFactory);
}

///////////////////////////////////////////////////////////////////////////////

RunTimeContainerImpl::RunTimeContainerImpl(const std::shared_ptr<RunTimeFactory>& runTimeFactory) {
	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); i++) {
		container.emplace_back(make_pair(IdType(i), runTimeFactory->create()));
	}
}

RunTimeContainerImpl::RunTimeContainerImpl(std::initializer_list<RunTimePtr> initializer) {
	if (ZoneHandler::getZoneCount() != initializer.size()) {
		throw std::logic_error("ZoneHandler::getZoneCount() != initializer.size()");
	}

	container.reserve(ZoneHandler::getZoneCount());
	for (size_t i = 0; i < ZoneHandler::getZoneCount(); ++i) {
		container.emplace_back(make_pair(IdType(i), *next(initializer.begin(), i)));
	}
}

RunTimeContainerImpl::const_mapped_type RunTimeContainerImpl::at(const key_type& key) const {
	if (container.size() <= key.getValue()) {
		throw NoSuchElementException("RunTime[" + key.toString() + "] does not exist");
	}

	return container[key.getValue()].second;
}

RunTimeContainerImpl::mapped_type RunTimeContainerImpl::at(const key_type& key) {
	if (container.size() <= key.getValue()) {
		throw NoSuchElementException("RunTime[" + key.toString() + "] does not exist");
	}

	return container[key.getValue()].second;
}

DurationList RunTimeContainerImpl::toDurationList() const {
	DurationList result;

	for (size_t i = 0; i < container.size(); i++) {
		result.emplace_back(container[i].second->toDuration());
	}

	return result;
}

std::list<RunTimeDTO> RunTimeContainerImpl::toRunTimeDtoList() const {
	std::list<RunTimeDTO> runTimeDtoList;
	for (size_t i = 0; i < container.size(); ++i) {
		runTimeDtoList.emplace_back(container[i].second->toRunTimeDto().setId(container[i].first.getValue()));
	}
	return runTimeDtoList;
}

void RunTimeContainerImpl::updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList) {
	if (runTimeDtoList.size() != container.size()) {
		throw IllegalArgumentException("RunTimeDtoList has to contain " + std::to_string(size()) + " elements");
	}

	auto it = runTimeDtoList.begin();
	for (size_t i = 0; i < container.size(); ++i, ++it) {
		const RunTimeDTO& runTimeDto = *it;

		if (runTimeDto.hasId()) {
			if (runTimeDto.getId() != i) {
				throw IllegalArgumentException(
					"The id attribute of the RunTimeDtoLists " + std::to_string(i) + "st/nd/rd item has to be " + std::to_string(i));
			}
		}

		container[i].second->updateFromRunTimeDto(runTimeDto);
	}
}

std::string RunTimeContainerImpl::toString() const {
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
