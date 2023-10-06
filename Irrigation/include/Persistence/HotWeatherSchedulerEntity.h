#pragma once
#include <memory>
#include <ostream>


class HotWeatherSchedulerEntity {
	const unsigned id;
	unsigned lastRun;

public:
	HotWeatherSchedulerEntity(unsigned id);
	HotWeatherSchedulerEntity(unsigned id, unsigned lastRun);

	bool operator==(const HotWeatherSchedulerEntity& other) const;

	unsigned getId() const     			{ return id; }
	unsigned getLastRun() const     	{ return lastRun; }

	void setLastRun(unsigned lastRun)	{ this->lastRun = lastRun; }

	std::string toString() const;
};

typedef std::shared_ptr<HotWeatherSchedulerEntity> HotWeatherSchedulerEntityPtr;

std::ostream& operator<<(std::ostream& os, const HotWeatherSchedulerEntity& obj);
std::ostream& operator<<(std::ostream& os, const HotWeatherSchedulerEntityPtr& obj);
