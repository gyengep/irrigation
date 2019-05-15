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
    	bool isDir;

		LOGGER.trace("Files found: %s/%s", basePath.c_str(), dp->d_name);
		LOGGER.trace("dp->d_type %u", (unsigned)dp->d_type);


	    LOGGER.trace("DT_UNKNOWN %d", DT_UNKNOWN);
	    LOGGER.trace("DT_FIFO %d", DT_FIFO);
	    LOGGER.trace("DT_CHR %d", DT_CHR);
	    LOGGER.trace("DT_DIR %d", DT_DIR);
	    LOGGER.trace("DT_BLK %d", DT_BLK);
	    LOGGER.trace("DT_REG %d", DT_REG);
	    LOGGER.trace("DT_LNK %d", DT_LNK);
	    LOGGER.trace("DT_SOCK %d", DT_SOCK);
	    LOGGER.trace("DT_WHT %d", DT_WHT);

        if (dp->d_type == DT_UNKNOWN || dp->d_type == DT_LNK) {
            struct stat stbuf;
            stat((basePath + '/' + dp->d_name).c_str(), &stbuf);
            isDir = S_ISDIR(stbuf.st_mode);
    		LOGGER.trace("stbuf.st_mode %u", (unsigned)stbuf.st_mode);

    		LOGGER.trace("S_ISBLK(m) %s", S_ISBLK(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISCHR(m) %s", S_ISCHR(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISDIR(m) %s", S_ISDIR(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISFIFO(m) %s", S_ISFIFO(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISREG(m) %s", S_ISREG(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISLNK(m) %s", S_ISLNK(stbuf.st_mode) ? "true" : "false");
    		LOGGER.trace("S_ISSOCK(m) %s", S_ISSOCK(stbuf.st_mode) ? "true" : "false");
        } else {
        	isDir = (dp->d_type == DT_DIR);
        }


		if (isDir) {

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
