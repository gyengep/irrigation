#pragma once
#include <memory>
#include <mutex>
#include "Utils/Timer.h"
#include "TemperatureException.h"

class TemperatureSensor;


class Temperature : public TimerCallback {
	static std::shared_ptr<Temperature> instance;

	const std::shared_ptr<TemperatureSensor> sensor;
	std::mutex mtx;
	Timer timer;
	bool valid;
	float value;

public:
	Temperature(const std::shared_ptr<TemperatureSensor>& sensor);
	virtual ~Temperature();

	void lock();
	void unlock();

	bool isValid() const;
	float getCachedValue() const;

	void startPeriodicRefresh();
	void stopPeriodicRefresh();
	void refresh();

	virtual void onTimer() override;

	static void init();
	static std::shared_ptr<Temperature> getInstancePtr();
};
