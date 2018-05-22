#include "Logger.h"
#include <iostream>
#include <fstream>
#include <thread>



#define LOGGER_FUNCTION(LEVEL)		\
	if (isLoggable(LEVEL)) {		\
		va_list args;				\
		va_start(args, format);		\
		log(LEVEL, format, args);	\
		va_end(args);				\
	}


unique_ptr<Logger> Logger::instance;
mutex Logger::initMutex;
const array<const char*, 6> Logger::levelTexts = {
	"OFF",
	"ERROR",
	"WARNING",
	"INFO",
	"DEBUG",
	"TRACE"
};


Logger& Logger::getInstance() {
	if (nullptr == instance.get()) {
		lock_guard<mutex> lock(initMutex);

		if (nullptr == instance) {
			instance.reset(new Logger());
		}
	}

	return *instance;
}

Logger::Logger() :
	logMutex(),
	level(OFF),
	output(nullptr),
	dynamicCreatedoutput()
{
	buffer[bufferSize] = '\0';
	setOutput(cout);
}

Logger::~Logger() {
}

void Logger::setFileName(const string& fileName) {
	ofstream* file = new ofstream(fileName, ios::out | ios::app);

	if (!file->is_open()) {
		delete file;
		runtime_error("Could not open file: " + fileName);
	}

	lock_guard<mutex> lock(logMutex);

	output = file;
	dynamicCreatedoutput.reset(file);
}

void Logger::setOutput(ostream& o) {
	lock_guard<mutex> lock(logMutex);

	output = &o;
	dynamicCreatedoutput.release();
}

void Logger::setLevel(Level level) {
	this->level = level;
}

const char* Logger::getLevelText(Level level) {
	try {
		return levelTexts.at(level);
	} catch (out_of_range& e) {
		throw out_of_range("Invalid log level: " + to_string(level));
	}
}

bool Logger::isLoggable(Level level) const {
	lock_guard<mutex> lock(logMutex);
	return (level <= this->level);
}

void Logger::log(Level level, const char * format, va_list args) {

	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);

	lock_guard<mutex> lock(logMutex);

	strftime(buffer, bufferSize, "%Y.%m.%d %H:%M:%S", timeinfo);
	(*output) << buffer << " ";
	(*output) << this_thread::get_id() << " ";
	(*output) << getLevelText(level) << ": ";

	vsnprintf(buffer, bufferSize, format, args);
	(*output) << buffer;

	(*output) << endl;
	(*output).flush();
}

void Logger::error(const char * format, ...)  {
	LOGGER_FUNCTION(ERROR);
}

void Logger::warning(const char * format, ...)  {
	LOGGER_FUNCTION(WARNING);
}

void Logger::info(const char * format, ...)  {
	LOGGER_FUNCTION(INFO);
}

void Logger::debug(const char * format, ...)  {
	LOGGER_FUNCTION(DEBUG);
}

void Logger::trace(const char * format, ...) {
	LOGGER_FUNCTION(TRACE);
}

