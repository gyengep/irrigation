#pragma once
#include "DurationList.h"


class WateringController {
public:
	virtual ~WateringController() = default;
	virtual bool isWateringActive() const = 0;
	virtual void start(const DurationList& durations) = 0;
	virtual void stop() = 0;
};
