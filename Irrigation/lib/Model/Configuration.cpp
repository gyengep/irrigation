#include "Configuration.h"
#include "Logger/Logger.h"

using namespace std;

const string Configuration::defaultConfigFileName = "/tmp/irrigation.xml";
const string Configuration::defaultLogFileName = "/tmp/irrigation.log";
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

