#include "Configuration.h"
#include "Logger/Logger.h"

using namespace std;
using namespace chrono;

const string Configuration::defaultConfigFileName = "/usr/share/irrigation/irrigation.xml";
const string Configuration::defaultLogFileName = "/var/log/irrigation/irrigation.log";
const duration<int64_t> Configuration::defaultTemperatureSensorUpdatePeriod = minutes(1);
const duration<int64_t> Configuration::defaultTemperatureCacheLength = hours(24);
const string Configuration::defaultTemperatureCacheFileName = "/usr/share/irrigation/temperature_cache.csv";
const duration<int64_t> Configuration::defaultTemperatureHistoryPeriod = hours(1);
const string Configuration::defaultTemperatureHistoryFileName = "/usr/share/irrigation/temperature.csv";

const LogLevel Configuration::defaultLogLevel = LogLevel::TRACE;


Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}
