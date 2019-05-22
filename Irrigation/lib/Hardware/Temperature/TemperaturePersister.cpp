#include "TemperaturePersister.h"
#include "Utils/CsvWriterImpl.h"
#include "Logger/Logger.h"
#include "Exceptions/Exceptions.h"
#include <fstream>

using namespace std;


TemperaturePersister::TemperaturePersister(const shared_ptr<ostream>& output, const shared_ptr<CsvWriterFactory>& csvWriterFactory) :
	output(output),
	csvWriter(csvWriterFactory->create(output)),
	timer(*this, chrono::seconds(60) * 60)
{
	if (output->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "Temperature"});
	}

	output->flush();
}

TemperaturePersister::~TemperaturePersister() {
	persistData();
}

void TemperaturePersister::append(time_t currentTime, float temperature) {
	unique_lock<mutex> lock(mtx);
	values.push_back(vector<string>{ timeToString(currentTime), temperatureToString(temperature) });
}

void TemperaturePersister::appendInvalid(time_t currentTime) {
	unique_lock<mutex> lock(mtx);
	values.push_back(vector<string>{ timeToString(currentTime), "N/A" });
}

void TemperaturePersister::persistData() {
	unique_lock<mutex> lock(mtx);
	if (!values.empty()) {
		deque<vector<string>> valuesCopy;
		valuesCopy.swap(values);
		lock.unlock();

		for (const auto& dateAndValue : valuesCopy) {
			csvWriter->append(dateAndValue);
		}

		output->flush();
	}
}

///////////////////////////////////////////////////////////////////////////////

void TemperaturePersister::startTimer() {
	timer.start();
}

void TemperaturePersister::stopTimer() {
	timer.stop();
}

void TemperaturePersister::onTimer() {
	persistData();
}

///////////////////////////////////////////////////////////////////////////////

string TemperaturePersister::timeToString(time_t time) {
	char buffer[32];
	const tm* ptm = localtime(&time);
	strftime(buffer, 32, "%Y.%m.%d %H:%M", ptm);
	return string(buffer);
}

string TemperaturePersister::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

shared_ptr<TemperaturePersister> TemperaturePersister::create(const string& fileName) {
	auto output = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);

	if (output->fail()) {
		throw IOException(errno);
	}

	LOGGER.debug("Temperature file: %s", fileName.c_str());
	return make_shared<TemperaturePersister>(output, make_shared<CsvWriterFactory>());
}

///////////////////////////////////////////////////////////////////////////////

shared_ptr<CsvWriter> TemperaturePersister::CsvWriterFactory::create(shared_ptr<ostream> output) {
	return make_shared<CsvWriterImpl>(output);
}
