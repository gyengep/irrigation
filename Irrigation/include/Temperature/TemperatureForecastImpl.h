#pragma once
#include <list>
#include <memory>
#include <mutex>
#include "Utils/Runnable.h"
#include "TemperatureForecast.h"
#include "TemperatureForecastProvider.h"


class TemperatureForecastImpl : public TemperatureForecast, public Runnable {
	const std::shared_ptr<TemperatureForecastProvider> provider;

	mutable std::mutex mtx;
	std::list<TemperatureForecastProvider::ValuesWithTimes> values;
	bool valid;

	void setValues(std::list<TemperatureForecastProvider::ValuesWithTimes>&& values);
	void invalidateValues();

public:
	TemperatureForecastImpl(const std::shared_ptr<TemperatureForecastProvider>& provider);
	virtual ~TemperatureForecastImpl();

	void updateCache();

	virtual void run() override { return updateCache(); }
	virtual Values getTemperatureForecast(const std::time_t& from, const std::time_t& to) const override;

	static void checkValueList(const std::list<TemperatureForecastProvider::ValuesWithTimes>& temperatures);

	// only for testing
	const std::list<TemperatureForecastProvider::ValuesWithTimes> getContainer() const;
};
