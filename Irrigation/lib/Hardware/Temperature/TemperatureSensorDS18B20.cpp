#include "TemperatureSensorDS18B20.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/FileReaderWriterImpl.h"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;


const string TemperatureSensor_DS18B20::basePath = "/sys/bus/w1/devices";
const string TemperatureSensor_DS18B20::fileName = "w1_slave";


string TemperatureSensor_DS18B20::getTempSensorFileName() {
	unique_ptr<DIR, int(*)(DIR*)> dirp(opendir(basePath.c_str()), closedir);

	if (dirp == nullptr) {
		throw runtime_error("DS18B20 temperature sensor file path does not exist: " + basePath);
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
			LOGGER.debug("DS18B20 temperature sensor file found: %s", result.c_str());
			return result;
    	}
    }

    throw runtime_error("DS18B20 temperature sensor file not found in path: " + basePath);
}

TemperatureSensor_DS18B20::TemperatureSensor_DS18B20() :
	TemperatureSensor_DS18B20(make_shared<FileReaderImpl>(getTempSensorFileName()))
{
}

TemperatureSensor_DS18B20::TemperatureSensor_DS18B20(const std::shared_ptr<FileReader>& fileReader) :
	fileReader(fileReader)
{
}

TemperatureSensor_DS18B20::~TemperatureSensor_DS18B20() {
}

float TemperatureSensor_DS18B20::readValueFromSensor() {
	const string text = fileReader->read();
	const size_t pos1 = text.find("t=");

	if (pos1 == string::npos) {
		throw TemperatureException("DS18B20 sensor file is invalid: " + text);
	}

	return stof(text.substr(pos1 + 2)) / 1000.0f;
}
