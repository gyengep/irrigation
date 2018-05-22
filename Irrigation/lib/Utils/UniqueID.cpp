#include "common.h"
#include "UniqueID.h"


std::mutex UniqueID::createMutex;
std::unique_ptr<UniqueID> UniqueID::instance;

UniqueID::UniqueID() :
	nextId(0)
{
}

UniqueID::~UniqueID() {
}

UniqueID& UniqueID::getInstance() {
	if (nullptr == instance) {
		std::lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new UniqueID());
		}
	}

	return *instance;
}

IdType UniqueID::getNextId() {
	return nextId++;
}

