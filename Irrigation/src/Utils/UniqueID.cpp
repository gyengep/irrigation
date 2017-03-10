#include "Common.h"
#include "UniqueID.h"

UniqueID* UniqueID::instance = NULL;

UniqueID::UniqueID() :
	nextId(0)
{
}

UniqueID::~UniqueID() {
	delete instance;
	instance = NULL;
}

UniqueID& UniqueID::getInstance() {
	if (NULL == instance) {
		instance = new UniqueID();
	}

	return *instance;
}

IdType UniqueID::getNextId() {
	return nextId++;
}

