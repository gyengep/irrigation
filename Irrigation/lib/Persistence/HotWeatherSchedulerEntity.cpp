#include "HotWeatherSchedulerEntity.h"
#include <sstream>


HotWeatherSchedulerEntity::HotWeatherSchedulerEntity(unsigned id) :
	id(id),
	lastRun(0)
{
}

HotWeatherSchedulerEntity::HotWeatherSchedulerEntity(unsigned id, unsigned lastRun):
	id(id),
	lastRun(lastRun)
{
}

std::string HotWeatherSchedulerEntity::toString() const {
	std::ostringstream oss;
	oss << "HotWeatherSchedulerEntity{";
	oss << "id=" << getId() << ", ";
	oss << "lastRun=" << getLastRun();
	oss << "}";
	return oss.str();
}

bool HotWeatherSchedulerEntity::operator==(const HotWeatherSchedulerEntity& other) const {
	return ((id == other.id) &&
		(lastRun == other.lastRun));
}

std::ostream& operator<<(std::ostream& os, const HotWeatherSchedulerEntity& obj) {
	os << "HotWeatherSchedulerEntity{ " << obj.toString() << " }";
	return os;
}

std::ostream& operator<<(std::ostream& os, const HotWeatherSchedulerEntityPtr& obj) {
	if (obj) {
		os << *obj;
	} else {
		os << "nullptr";
	}

	return os;
}
