#include "Configuration.h"
#include "Logger/Logger.h"

using namespace std;

const string Configuration::defaultConfigFileName = "/usr/share/irrigation/irrigation.xml";
const string Configuration::defaultLogFileName = "/var/log/irrigation/irrigation.log";
const LogLevel Configuration::defaultLogLevel = LogLevel::TRACE;


Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}
