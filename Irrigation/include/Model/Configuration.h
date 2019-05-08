#pragma once
#include <string>
#include <memory>
#include <mutex>

enum class LogLevel;



class Configuration {
	static const std::string defaultConfigFileName;
	static const std::string defaultLogFileName;
	static const LogLevel defaultLogLevel;
	static const uint16_t defaultRestPort = 8080;

public:
	Configuration() = default;
	~Configuration() = default;

	const std::string& getConfigFileName() const { return defaultConfigFileName; }
	const std::string& getLogFileName() const { return defaultLogFileName; }
	LogLevel getLogLevel() const { return defaultLogLevel; }
	uint16_t getRestPort() const { return defaultRestPort; }

	static Configuration& getInstance();
};
