#include "Temperature.h"
#include "Utils/FileReaderWriterImpl.h"
#include "Logger/Logger.h"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>



#include <dirent.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


const string Temperature::basePath = "/sys/bus/w1/devices";
//const string Temperature::basePath = "/home/gyenge/sys/bus/w1/devices";
const string Temperature::fileName = "w1_slave";
shared_ptr<Temperature> Temperature::instance;


void Temperature::init() {
	try {
		instance = make_shared<Temperature>(
				getTempSensorFileName(),
				make_shared<FileReaderImpl>()
				);

		instance->refresh();
		instance->startPeriodicRefresh();
	} catch (const exception& e) {
		LOGGER.warning(e.what());
		instance = make_shared<Temperature>("", shared_ptr<FileReaderImpl>());
	}
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

string Temperature::getTempSensorFileName() {
	unique_ptr<DIR, int(*)(DIR*)> dirp(opendir(basePath.c_str()), closedir);

	if (dirp == nullptr) {
		throw runtime_error("Temperature sensor file path does not exist: " + basePath);
	}

    struct dirent* dp;
    while ((dp = readdir(dirp.get())) != NULL) {
    	const string filePath = basePath + '/' + dp->d_name;
    	bool isDir;

        if (dp->d_type == DT_UNKNOWN || dp->d_type == DT_LNK) {
            struct stat stbuf;
            stat(filePath.c_str(), &stbuf);
            isDir = S_ISDIR(stbuf.st_mode);
        } else {
        	isDir = (dp->d_type == DT_DIR);
        }

		if (true == isDir && 0 == strncmp(dp->d_name, "28-", 3)) {
			const string result = filePath + '/' + fileName;
			LOGGER.debug("Temperature sensor file found: %s", result.c_str());
			return result;
    	}
    }

    throw runtime_error("Temperature sensor file not found in path: " + basePath);
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
	timer.reset(new Timer(*this, chrono::seconds(60)));
	timer->start();
}

void Temperature::stopPeriodicRefresh() {
	timer.reset();
}

void Temperature::onTimer() {
	lock_guard<mutex> lock(mtx);
	refresh();
}
