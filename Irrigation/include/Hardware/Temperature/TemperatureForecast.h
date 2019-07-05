#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include "Utils/Timer.h"
#include "TemperatureForecastProvider.h"


class TemperatureForecast : public TimerCallback {
public:
	struct Values;

private:
	const std::shared_ptr<TemperatureForecastProvider> provider;

	mutable std::mutex mtx;
	std::list<TemperatureForecastProvider::ValuesWithTimes> temperatures;
	std::unique_ptr<Timer> timer;

public:
	TemperatureForecast(const std::shared_ptr<TemperatureForecastProvider>& provider);
	virtual ~TemperatureForecast();

	Values getForecastValues(std::time_t from, std::time_t to) const;
	void updateCache();

	void startTimer(const std::chrono::duration<int64_t>& period);
	void stopTimer();
	virtual void onTimer() override;

	static void checkValueList(const std::list<TemperatureForecastProvider::ValuesWithTimes>& temperatures);

	// only for testing
	const std::list<TemperatureForecastProvider::ValuesWithTimes> getContainer() const;

};

struct TemperatureForecast::Values {
	const float min;
	const float max;

	Values(float min, float max);

	// for testing
	bool operator== (const Values& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Values& values);
};
