#pragma once
#include <condition_variable>
#include <memory>
#include <thread>
#include <vector>
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

	void workerFunc(const std::vector<RunTimePtr> runTimes);

public:
	WateringController();
	WateringController(WateringController&&) = delete;
	WateringController(const WateringController&) = delete;
	WateringController(const std::shared_ptr<ZoneHandler>& zoneHandler);
	virtual ~WateringController();

	WateringController& operator= (WateringController&&) = delete;
	WateringController& operator= (const WateringController&) = delete;

	virtual void start(const RunTimeContainer& runTimes, unsigned adjustmentPercent);
	virtual void stop();

	virtual bool isWateringActive() const;
	size_t getActiveZoneId() const;

	static std::vector<RunTimePtr> adjustRunTimes(const RunTimeContainer& runTimes, unsigned adjustmentPercent);
};
