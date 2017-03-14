#include "Common.h"
#include "UniqueID.h"

std::unique_ptr<UniqueID> UniqueID::instance;

UniqueID::UniqueID() :
	nextId(0)
{
}

UniqueID::~UniqueID() {
}

UniqueID& UniqueID::getInstance() {
	if (nullptr == instance) {
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);

		if (nullptr == instance) {
			instance.reset(new UniqueID());
		}
	}

	return *instance;
}

IdType UniqueID::getNextId() {
	return nextId++;
}

