#pragma once
#include <condition_variable>
#include <list>
#include <memory>
#include <thread>
#include "Hardware/Valves/ZoneHandler.h"
#include "RunTime.h"
#include "RunTimeContainer.h"


class WateringController {

	const std::shared_ptr<ZoneHandler> zoneHandler;
	std::thread workerThread;
	std::condition_variable condition;
	mutable std::mutex mtx;
	bool stopped;
	bool active;

	void workerFunc(const std::list<std::chrono::milliseconds> runTimes);

public:
	WateringController();
	WateringController(WateringController&&) = delete;
	WateringController(const WateringController&) = delete;
	WateringController(const std::shared_ptr<ZoneHandler>& zoneHandler);
	virtual ~WateringController();

	WateringController& operator= (WateringController&&) = delete;
	WateringController& operator= (const WateringController&) = delete;

	virtual void start(const std::list<std::chrono::milliseconds>& runTimes, unsigned adjustmentPercent);
	virtual void start(const std::list<std::chrono::seconds>& runTimes, unsigned adjustmentPercent);
	virtual void stop();

	virtual bool isWateringActive() const;
	size_t getActiveZoneId() const;

	static std::list<std::chrono::milliseconds> adjustRunTimes(const std::list<std::chrono::milliseconds>& runTimes, unsigned adjustmentPercent);
};
