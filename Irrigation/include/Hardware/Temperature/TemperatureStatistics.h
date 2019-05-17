#pragma once
#include <memory>
#include "Utils/Timer.h"

class Temperature;
class TemperatureFileHandler;


class TemperatureStatistics : public TimerCallback {
	const std::shared_ptr<Temperature> temperature;
	const std::shared_ptr<TemperatureFileHandler> temperatureFileHandler;
	Timer timer;

public:
	TemperatureStatistics(const std::shared_ptr<Temperature>& temperature, const std::shared_ptr<TemperatureFileHandler>& temperatureFileHandler);
	virtual ~TemperatureStatistics();

	void start();
	void stop();
	void requestTemperature();

	virtual void onTimer() override;
};
