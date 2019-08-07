#pragma once
#include <list>
#include <memory>
#include <mutex>
#include "Utils/Timer.h"
#include "TemperatureForecast.h"
#include "TemperatureForecastProvider.h"


class TemperatureForecastImpl : public TemperatureForecast, public TimerCallback {
	const std::shared_ptr<TemperatureForecastProvider> provider;

	mutable std::mutex mtx;
	std::list<TemperatureForecastProvider::ValuesWithTimes> temperatures;
	std::unique_ptr<Timer> timer;

public:
	TemperatureForecastImpl(const std::shared_ptr<TemperatureForecastProvider>& provider);
	virtual ~TemperatureForecastImpl();

	virtual Values getForecastValues(const std::time_t& from, const std::time_t& to) const override;
	void updateCache();

	void startTimer(const std::chrono::seconds& period);
	void stopTimer();
	virtual void onTimer() override;

	static void checkValueList(const std::list<TemperatureForecastProvider::ValuesWithTimes>& temperatures);

	// only for testing
	const std::list<TemperatureForecastProvider::ValuesWithTimes> getContainer() const;

};