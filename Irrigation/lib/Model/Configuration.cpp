#include "Configuration.h"
#include "Logger/Logger.h"

const std::string Configuration::defaultConfigFileName = "/usr/share/irrigation/irrigation.xml";
const std::string Configuration::defaultPersistedDataFileName = "/usr/share/irrigation/persist.json";
const std::string Configuration::defaultLogFileName = "/var/log/irrigation/irrigation.log";
const std::string Configuration::defaultAccessLogFileName = "/var/log/irrigation/access_log.txt";
const std::string Configuration::defaultTemperatureCacheFileName = "/usr/share/irrigation/temperature_cache.csv";
const std::string Configuration::defaultTemperatureHistoryFileName = "/usr/share/irrigation/temperature.csv";
const std::chrono::duration<int64_t> Configuration::defaultCurrentTemperatureUpdatePeriod = std::chrono::minutes(5);
const std::chrono::duration<int64_t> Configuration::defaultTemperatureForecastUpdatePeriod = std::chrono::hours(1);
const std::chrono::duration<int64_t> Configuration::defaultTemperatureCacheLength = std::chrono::hours(24);
const std::chrono::duration<int64_t> Configuration::defaultTemperatureHistoryPeriod = std::chrono::hours(1);
const std::string Configuration::defaultResourceDirectory = "/usr/share/irrigation/web_root";
//const string Configuration::defaultResourceDirectory = "/cygdrive/c/Work/irrigation/Irrigation/web_root";

const LogLevel Configuration::defaultLogLevel = LogLevel::TRACE;


Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}
