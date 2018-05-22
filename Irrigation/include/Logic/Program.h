#pragma once
#include <ctime>
#include <memory>
#include <string>
#include <vector>

class Scheduler;
class SpecifiedScheduler;
class RunTimeContainer;
class StartTimeContainer;
class SchedulerFactory;


class Program {
public:

	enum SchedulerType {
		SPECIFIED_DAYS,
	};

private:

	// disable copy constructor and copy operator
	Program(const Program&);
	void operator= (const Program&);

	void initSchedulers(std::unique_ptr<const SchedulerFactory> schedulerFactory);

	std::string name;
	SchedulerType schedulerType;
	std::vector<std::unique_ptr<Scheduler>> schedulers;
	std::unique_ptr<RunTimeContainer> runTimes;
	std::unique_ptr<StartTimeContainer> startTimes;

public:
	Program(const std::string& name);
	virtual ~Program();

	// for testing
	Program(const SchedulerFactory* schedulerFactory = nullptr);

	std::string getName() const;
	virtual void setName(const std::string& name);

	void setSchedulerType(SchedulerType schedulerType);
	SchedulerType getSchedulerType() const;
	const Scheduler& getCurrentScheduler() const;

	const SpecifiedScheduler& getSpecifiedScheduler() const;
	SpecifiedScheduler& getSpecifiedScheduler();

	bool isScheduled(const std::time_t& rawTime) const;

	const RunTimeContainer& getRunTimes() const { return *runTimes; }
	RunTimeContainer& getRunTimes() { return *runTimes; }
	const StartTimeContainer& getStartTimes() const { return *startTimes; }
	StartTimeContainer& getStartTimes() { return *startTimes; }
};


class SchedulerFactory {
public:
	virtual ~SchedulerFactory() {}
	virtual Scheduler* createScheduler(Program::SchedulerType schedulerType) const;
};
