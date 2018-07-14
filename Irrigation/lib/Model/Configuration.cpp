#include "Configuration.h"
#include "Logger/Logger.h"

using namespace std;

unique_ptr<Configuration> Configuration::instance;
mutex Configuration::createMutex;

const string Configuration::defaultConfigFileName = "/usr/share/irrigation/irrigation.xml";
const string Configuration::defaultLogFileName = "/var/log/irrigation/irrigation.log";
const LogLevel Configuration::defaultLogLevel = LogLevel::TRACE;


Configuration& Configuration::getInstance() {
	if (nullptr == instance) {
		lock_guard<mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Configuration());
		}
	}

	return *instance.get();
}

Configuration::Configuration() {
}

Configuration::~Configuration() {
}

