#include "Configuration.h"
#include "Logger/Logger.h"

using namespace std;
using namespace chrono;

const string Configuration::defaultConfigFileName = "/usr/share/irrigation/irrigation.xml";
const string Configuration::defaultPersistedDataFileName = "/usr/share/irrigation/persist.json";
const string Configuration::defaultLogFileName = "/var/log/irrigation/irrigation.log";
const string Configuration::defaultTemperatureCacheFileName = "/usr/share/irrigation/temperature_cache.csv";
const string Configuration::defaultTemperatureHistoryFileName = "/usr/share/irrigation/temperature.csv";
const duration<int64_t> Configuration::defaultCurrentTemperatureUpdatePeriod = minutes(5);
const duration<int64_t> Configuration::defaultTemperatureForecastUpdatePeriod = hours(1);
const duration<int64_t> Configuration::defaultTemperatureCacheLength = hours(24);
const duration<int64_t> Configuration::defaultTemperatureHistoryPeriod = hours(1);
const string Configuration::defaultResourceDirectory = "/usr/share/irrigation/web_root";
//const string Configuration::defaultResourceDirectory = "/cygdrive/c/Work/irrigation/Irrigation/web_root";

const LogLevel Configuration::defaultLogLevel = LogLevel::TRACE;


Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}
