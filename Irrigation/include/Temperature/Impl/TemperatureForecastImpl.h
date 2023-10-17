#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <vector>
#include "Utils/Thread.h"
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureForecastProvider.h"


class TemperatureForecastImpl : public TemperatureForecast {
	const std::shared_ptr<TemperatureForecastProvider> provider;

	std::unique_ptr<Thread> workerThread;
	mutable std::mutex mtx;
	std::list<TemperatureForecastProvider::ValuesWithTimes> values;
	bool valid;

	void setValues(std::list<TemperatureForecastProvider::ValuesWithTimes>&& values);
	void invalidateValues();

public:
	TemperatureForecastImpl(const std::shared_ptr<TemperatureForecastProvider>& provider);
	virtual ~TemperatureForecastImpl();

	void updateCache();

	void start(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed);
	void stop();

	virtual Values getTemperatureForecast(const DateTime& from, const DateTime& to) const override;
	virtual TemperatureForecastDTO toTemperatureForecastDTO(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat) const override;

	static void checkValueList(const std::list<TemperatureForecastProvider::ValuesWithTimes>& temperatures);

	// only for testing
	const std::list<TemperatureForecastProvider::ValuesWithTimes> getContainer() const;
};
