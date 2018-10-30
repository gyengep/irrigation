#pragma once
#include <ctime>
#include <memory>
#include <vector>
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "Hardware/ZoneHandler.h"


class WateringController {

	struct WateringProperties {
		std::time_t zoneStartTime;
		std::vector<RunTime> runTimes;

		WateringProperties();
	};

	std::unique_ptr<WateringProperties> wateringProperties;
	std::shared_ptr<ZoneHandler> zoneHandler;

	void startNextRequiredZone(const std::time_t& rawTime);

public:
	WateringController();
	WateringController(std::shared_ptr<ZoneHandler> zoneHandler);
	virtual ~WateringController();

	virtual void on1SecTimer(const std::time_t& rawTime);
	virtual void start(const std::time_t& rawTime, const RunTimeContainer& runTimes, unsigned adjustmentPercent = 100);
	virtual void stop();

	virtual bool isWateringActive() const;
	size_t getActiveZoneId() const;
};
