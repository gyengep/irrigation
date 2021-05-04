#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include "TemperatureException.h"

class CurrentTemperatureProvider;
class TemperatureForecast;
class TemperatureHistory;
class CurrentTemperatureImpl;
class TemperatureForecastImpl;
class TemperatureHistoryImpl;
class TemperatureHistoryLogger;


class Temperature {
	std::shared_ptr<CurrentTemperatureImpl> current;
	std::shared_ptr<TemperatureForecastImpl> forecast;
	std::shared_ptr<TemperatureHistoryImpl> history;
	std::shared_ptr<TemperatureHistoryLogger> historyLogger;

	std::shared_ptr<CurrentTemperatureProvider> createCurrentTemperatureProvider();

	Temperature();

public:

	struct CurrentTemperatureProperties {
		std::chrono::milliseconds updatePeriod;
		std::vector<std::chrono::milliseconds> delayOnFailed;

		CurrentTemperatureProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed);
	};

	struct TemperatureForecastProperties {
		std::chrono::milliseconds updatePeriod;
		std::vector<std::chrono::milliseconds> delayOnFailed;

		TemperatureForecastProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed);
	};

	struct TemperatureHistoryProperties {
		std::chrono::seconds length;
		std::string fileName;

		TemperatureHistoryProperties(const std::chrono::seconds& length, const std::string& fileName);
	};

	struct TemperatureHistoryLoggerProperties {
		std::chrono::milliseconds period;
		std::string fileName;

		TemperatureHistoryLoggerProperties(const std::chrono::milliseconds& period, const std::string& fileName);
	};

	~Temperature();

	const std::shared_ptr<TemperatureHistory> getTemperatureHistory() const;
	const std::shared_ptr<TemperatureForecast> getTemperatureForecast() const;

	void init(
			const CurrentTemperatureProperties& currentTemperatureProperties,
			const TemperatureForecastProperties& temperatureForecastProperties,
			const TemperatureHistoryProperties& temperatureHistoryProperties,
			const TemperatureHistoryLoggerProperties& temperatureHistoryLoggerProperties
		);
	void uninit();

	static Temperature& getInstance();
};
