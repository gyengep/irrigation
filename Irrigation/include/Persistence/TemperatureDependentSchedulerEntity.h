#pragma once
#include <memory>
#include <ostream>


class TemperatureDependentSchedulerEntity {
	const unsigned id;
	int remainingPercent;
	unsigned lastRun;

public:
	TemperatureDependentSchedulerEntity(unsigned id);
	TemperatureDependentSchedulerEntity(unsigned id, int remainingPercent, unsigned lastRun);

	bool operator==(const TemperatureDependentSchedulerEntity& other) const;

	unsigned getId() const 							{ return id; }
	int getRemainingPercent() const 				{ return remainingPercent; }
	unsigned getLastRun() const     				{ return lastRun; }

	void setRemainingPercent(int remainingPercent)	{ this->remainingPercent = remainingPercent; }
	void setLastRun(unsigned lastRun)             	{ this->lastRun = lastRun; }

	std::string toString() const;
};

typedef std::shared_ptr<TemperatureDependentSchedulerEntity> TemperatureDependentSchedulerEntityPtr;

std::ostream& operator<<(std::ostream& os, const TemperatureDependentSchedulerEntity& obj);
std::ostream& operator<<(std::ostream& os, const TemperatureDependentSchedulerEntityPtr& obj);
