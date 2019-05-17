#include "TemperatureFileHandler.h"
#include "Utils/CsvWriterImpl.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureFileHandler::TemperatureFileHandler(const string& filePath) :
	fileName(createFileName(filePath)),
	timer(*this, chrono::seconds(60))
{
	LOGGER.debug("Temperature file: %s", fileName.c_str());

	CsvWriterImpl csvWriter(make_shared<ofstream>(fileName, ofstream::out), ',');
	csvWriter.append(vector<string>{"Date", "Temperature"});
}

TemperatureFileHandler::~TemperatureFileHandler() {
	timer.stop();
	saveToFile();
}

void TemperatureFileHandler::start() {
	timer.start();
}

void TemperatureFileHandler::stop() {
	timer.stop();
}

void TemperatureFileHandler::append(time_t currentTime, float temperature) {
	unique_lock<mutex> lock(mtx);
	values.push_back(vector<string>{ timeToString(currentTime), temperatureToString(temperature) });
}

void TemperatureFileHandler::appendInvalid(time_t currentTime) {
	unique_lock<mutex> lock(mtx);
}

void TemperatureFileHandler::onTimer() {
	LOGGER.debug("TemperatureFileHandler::onTimer()");
	saveToFile();
}

void TemperatureFileHandler::saveToFile() {
	deque<vector<string>> valuesCopy;

	{
		unique_lock<mutex> lock(mtx);
		valuesCopy.swap(values);
	}

	auto ofs = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);
	saveValues(make_shared<CsvWriterImpl>(ofs, ','), valuesCopy);
}

void TemperatureFileHandler::saveValues(const shared_ptr<CsvWriter>& csvWriter, deque<vector<string>> values) {
	for (const auto& dateAndValue : values) {
		csvWriter->append(dateAndValue);
	}
}

string TemperatureFileHandler::timeToString(time_t time) {
	char buffer[32];
	const tm* ptm = localtime(&time);
	strftime(buffer, 32, "%Y.%m.%d %H:%M", ptm);
	return string(buffer);
}

string TemperatureFileHandler::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

string TemperatureFileHandler::createFileName(const string& filePath) {
	char buffer[40];
	time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	const tm* ptm = localtime(&time);
	strftime(buffer, 32, "/temperature_%Y%m%d%H%M%S.csv", ptm);

	return filePath + buffer;
}
