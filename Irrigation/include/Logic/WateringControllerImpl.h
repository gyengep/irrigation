#pragma once
#include <condition_variable>
#include <memory>
#include <thread>
#include "Hardware/Valves/ZoneHandler.h"
#include "DurationList.h"
#include "WateringController.h"


class WateringControllerImpl : public WateringController {

	const std::shared_ptr<ZoneHandler> zoneHandler;
	std::thread workerThread;
	std::condition_variable condition;
	mutable std::mutex mtx;
	bool stopped;
	bool active;

	void workerFunc(const std::list<std::chrono::milliseconds> durations);

public:
	WateringControllerImpl(const std::shared_ptr<ZoneHandler>& zoneHandler);
	virtual ~WateringControllerImpl();

	void startMs(const std::list<std::chrono::milliseconds>& durations);
	virtual void start(const DurationList& durations) override;
	virtual void stop() override;

	virtual bool isWateringActive() const override;
};
