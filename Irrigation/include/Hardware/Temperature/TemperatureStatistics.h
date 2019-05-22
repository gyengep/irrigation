#pragma once
#include <memory>
#include "Utils/Timer.h"

class TemperatureSensor;
class TemperaturePersister;


class TemperatureStatistics : public TimerCallback {
	const std::shared_ptr<TemperatureSensor> temperatureSensor;
	const std::shared_ptr<TemperaturePersister> temperaturePersister;
	Timer timer;

public:
	TemperatureStatistics(const std::shared_ptr<TemperatureSensor>& temperatureSensor, const std::shared_ptr<TemperaturePersister>& temperaturePersister);
	virtual ~TemperatureStatistics();

	void requestTemperature();

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;
};
