#pragma once
#include <chrono>
#include <string>
#include <memory>
#include <mutex>

enum class LogLevel;



class Configuration {
	static const std::string defaultConfigFileName;
	static const std::string defaultLogFileName;
	static const std::string defaultTemperatureCacheFileName;
	static const std::string defaultTemperatureHistoryFileName;
	static const std::chrono::duration<int64_t> defaultTemperatureSensorUpdatePeriod;
	static const std::chrono::duration<int64_t> defaultTemperatureForecastUpdatePeriod;
	static const std::chrono::duration<int64_t> defaultTemperatureCacheLength;
	static const std::chrono::duration<int64_t> defaultTemperatureHistoryPeriod;
	static const LogLevel defaultLogLevel;
	static const uint16_t defaultRestPort = 8080;

public:
	Configuration() = default;
	~Configuration() = default;

	const std::string& getConfigFileName() const { return defaultConfigFileName; }
	const std::string& getLogFileName() const { return defaultLogFileName; }
	const std::string& getTemperatureCacheFileName() const { return defaultTemperatureCacheFileName; }
	const std::string& getTemperatureHistoryFileName() const { return defaultTemperatureHistoryFileName; }
	const std::chrono::duration<int64_t>& getTemperatureSensorUpdatePeriod() const { return defaultTemperatureSensorUpdatePeriod; }
	const std::chrono::duration<int64_t>& getTemperatureForecastUpdatePeriod() const { return defaultTemperatureForecastUpdatePeriod; }
	const std::chrono::duration<int64_t>& getTemperatureCacheLength() const { return defaultTemperatureCacheLength; }
	const std::chrono::duration<int64_t>& getTemperatureHistoryPeriod() const { return defaultTemperatureHistoryPeriod; }
	LogLevel getLogLevel() const { return defaultLogLevel; }
	uint16_t getRestPort() const { return defaultRestPort; }

	static Configuration& getInstance();
};
