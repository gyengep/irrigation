#pragma once
#include <string>
#include <memory>
#include <mutex>

enum class LogLevel;



class Configuration {
	static const std::string defaultConfigFileName;
	static const std::string defaultLogFileName;
	static const LogLevel defaultLogLevel;

public:
	Configuration() = default;
	~Configuration() = default;

	std::string getConfigFileName() const { return defaultConfigFileName; }
	std::string getLogFileName() const { return defaultLogFileName; }
	LogLevel getLogLevel() const { return defaultLogLevel; }

	static Configuration& getInstance();
};
