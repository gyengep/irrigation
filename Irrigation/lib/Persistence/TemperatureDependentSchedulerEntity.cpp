#include "TemperatureDependentSchedulerEntity.h"
#include <sstream>


TemperatureDependentSchedulerEntity::TemperatureDependentSchedulerEntity(unsigned id) :
	id(id),
	remainingPercent(0),
	lastRun(0)
{
}

TemperatureDependentSchedulerEntity::TemperatureDependentSchedulerEntity(unsigned id, int remainingPercent, unsigned lastRun):
	id(id),
	remainingPercent(remainingPercent),
	lastRun(lastRun)
{
}

std::string TemperatureDependentSchedulerEntity::toString() const {
	std::ostringstream oss;
	oss << "TemperatureDependentSchedulerEntity{";
	oss << "id=" << getId() << ", ";
	oss << "remainingPercent=" << getRemainingPercent() << ", ";
	oss << "lastRun=" << getLastRun();
	oss << "}";
	return oss.str();
}

bool TemperatureDependentSchedulerEntity::operator==(const TemperatureDependentSchedulerEntity& other) const {
	return ((id == other.id) &&
		(remainingPercent == other.remainingPercent) &&
		(lastRun == other.lastRun));
}

std::ostream& operator<<(std::ostream& os, const TemperatureDependentSchedulerEntity& obj) {
	os << "TemperatureDependentSchedulerEntity{ " << obj.toString() << " }";
	return os;
}

std::ostream& operator<<(std::ostream& os, const TemperatureDependentSchedulerEntityPtr& obj) {
	if (obj) {
		os << *obj;
	} else {
		os << "nullptr";
	}

	return os;
}
