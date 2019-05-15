#include "Temperature.h"
#include "Utils/FileReaderWriterImpl.h"
#include "Logger/Logger.h"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;


const string Temperature::basePath = "/sys/bus/w1/devices";
//const string Temperature::basePath = "/home/gyenge/sys/bus/w1/devices";
const string Temperature::fileName = "w1_slave";
shared_ptr<Temperature> Temperature::instance;


void Temperature::init() {
	instance.reset(new Temperature(
			getTempSensorFileName(),
			make_shared<FileReaderImpl>()
			));

	instance->startPeriodicRefresh();
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

string Temperature::getTempSensorFileName() {
	unique_ptr<DIR, int(*)(DIR*)> dirp(opendir(basePath.c_str()), closedir);

	if (dirp == nullptr) {
		LOGGER.warning("Temperature sensor file path does not exist: %s", basePath.c_str());
	    return string();
	}

    struct dirent* dp;

    while ((dp = readdir(dirp.get())) != NULL) {
		LOGGER.trace("Files found: %s/%s %d", basePath.c_str(), dp->d_name, (int)dp->d_type);

		if (S_ISDIR(dp->d_type)) {

        	if (0 == strncmp(dp->d_name, "28-", 3)) {
        		const string result = basePath + '/' + dp->d_name + '/' + fileName;
        		LOGGER.debug("Temperature sensor file found: %s", result.c_str());
        		return result;
        	}
    	}
    }

	LOGGER.warning("Temperature sensor file not found in path: %s", basePath.c_str());
    return string();
}

Temperature::Temperature(const std::string& fileName, const std::shared_ptr<FileReader>& fileReader) :
	temperatureSensorFileName(fileName),
	fileReader(fileReader),
	valid(false),
	value(0.0f)
{
}

Temperature::~Temperature() {
}

void Temperature::lock() {
	mtx.lock();
}

void Temperature::unlock() {
	mtx.unlock();
}

float Temperature::readValueFromSensor() {
	const string text = fileReader->read(temperatureSensorFileName);
	const size_t pos1 = text.find("t=");

	if (pos1 == string::npos) {
		throw TemperatureException("Invalid temperature file content: " + text);
	}

	return stof(text.substr(pos1 + 2)) / 1000.0f;
}

void Temperature::refresh() {
	try {
		value = readValueFromSensor();
		valid = true;
		LOGGER.debug("Temperature: %f", value);
	} catch(const exception& e) {
		valid = false;
	}
}

bool Temperature::isValid() const {
	return valid;
}

float Temperature::getCachedValue() const {
	if (!valid) {
		throw logic_error("Temperature::getCachedValue() !valid");
	}

	return value;
}

void Temperature::startPeriodicRefresh() {
	timer.reset(new Timer(*this, chrono::seconds(600)));
	timer->start();
	refresh();
}

void Temperature::stopPeriodicRefresh() {
	timer.reset();
}

void Temperature::onTimer() {
	lock_guard<mutex> lock(mtx);
	refresh();
}
