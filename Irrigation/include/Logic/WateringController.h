#pragma once
#include <array>
#include <ctime>
#include <memory>
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "Hardware/ZoneHandler.h"


class WateringController {

	struct WateringProperties {
		std::time_t zoneStartTime;
		std::array<RunTime, ZONE_COUNT> runTimes;

		WateringProperties();
	};

	std::unique_ptr<WateringProperties> wateringProperties;
	std::shared_ptr<ZoneHandler> zoneHandler;

	void startNextRequiredZone(const std::time_t& rawTime);

public:
	WateringController(std::shared_ptr<ZoneHandler> zoneHandler);
	virtual ~WateringController();

	void on1SecTimer(const std::time_t& rawTime);
	void start(const std::time_t& rawTime, const RunTimeContainer& runTimes, float adjustment = 1.0f);
	void stop();

	bool isWateringActive() const;
	size_t getActiveZoneId() const;
};
