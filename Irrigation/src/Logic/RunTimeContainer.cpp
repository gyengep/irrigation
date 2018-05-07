#include "RunTimeContainer.h"
#include <memory>
#include "Logger/Logger.h"



RunTimeContainer::RunTimeContainer() {
	init(new RunTimeFactory());
}

RunTimeContainer::RunTimeContainer(RunTimeFactory* runTimeFactory) {
	init(runTimeFactory);
}

RunTimeContainer::~RunTimeContainer() {
	for (auto it = container.begin(); it != container.end(); ++it) {
		delete it->second;
	}
}

void RunTimeContainer::init(RunTimeFactory* runTimeFactory) {
	std::unique_ptr<RunTimeFactory> runTimeFactoryPtr(runTimeFactory);

	container.reserve(ZONE_COUNT);
	for (size_t i = 0; i < ZONE_COUNT; i++) {
		container.push_back(std::make_pair(i, runTimeFactory->createRunTime()));
	}
}

const RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) const {
	if (container.size() <= key) {
		throw InvalidRunTimeIdException(key);
	}

	return container[key].second;
}

RunTimeContainer::mapped_type& RunTimeContainer::at(const key_type& key) {
	if (container.size() <= key) {
		throw InvalidRunTimeIdException(key);
	}

	return container[key].second;
}
