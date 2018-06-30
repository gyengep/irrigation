#include "Logger.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;


#define LOGGER_FUNCTION_VA(LEVEL)							\
	if (isLoggable(LEVEL)) {								\
		const size_t messageSize = 1000;					\
		char message[messageSize + 1];						\
		va_list args;										\
		va_start(args, format);								\
		vsnprintf(message, messageSize, format, args);		\
		va_end(args);										\
		log(LEVEL, message, nullptr);						\
	}

//va_list args1, args2;
//va_start(args1, format);
//va_copy(args2, args1);
//vector<char> buffer(1 + std::vsnprintf(nullptr, 0, format, args1));
//va_end(args1);
//std::vsnprintf(buffer.data(), buffer.size(), format, args2);
//va_end(args2);

#define LOGGER_FUNCTION_EXCEPTION(LEVEL)					\
	if (isLoggable(LEVEL)) {								\
		log(LEVEL, message, &e);							\
	}

unique_ptr<Logger> Logger::instance;
mutex Logger::createMutex;


string to_string(LogLevel logLevel) {
	switch(logLevel) {
	case LogLevel::OFF:
		return "OFF";
	case LogLevel::ERROR:
		return "ERROR";
	case LogLevel::WARNING:
		return "WARNING";
	case LogLevel::INFO:
		return "INFO";
	case LogLevel::DEBUG:
		return "DEBUG";
	case LogLevel::TRACE:
		return "TRACE";
	default:
		throw invalid_argument("Unknown value of enum LogLevel: " + to_string(static_cast<unsigned>(logLevel)));
	}
}


Logger& Logger::getInstance() {
	if (nullptr == instance.get()) {
		lock_guard<mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Logger());
		}
	}

	return *instance;
}

Logger::Logger() :
	logMutex(),
	logLevel(LogLevel::OFF),
	output(nullptr)
{
}

Logger::~Logger() {
}

void Logger::setOutput(ostream* o) {
	lock_guard<mutex> lock(logMutex);
	output.reset(o);
}

void Logger::setLevel(LogLevel logLevel) {
	this->logLevel = logLevel;
}

bool Logger::isLoggable(LogLevel logLevel) const {
	return (logLevel <= this->logLevel);
}

string Logger::logException(const exception* e, unsigned level) {
	ostringstream o;

	if (nullptr != e) {
		o << endl << string(level * 2, ' ') << "exception: " << e->what();

	    try {
	        rethrow_if_nested(*e);
	    } catch(const exception& innerException) {
	    	o << logException(&innerException, level + 1);
	    }
	}

    return o.str();
}

void Logger::log(LogLevel logLevel, const char* message, const exception* e) {

	lock_guard<mutex> lock(logMutex);

	if (output.get() != nullptr) {
		time_t t = time(nullptr);

		(*output) << put_time(localtime(&t), "%Y.%m.%d %H:%M:%S") << ' ';
		(*output) << this_thread::get_id() << " ";
		(*output) << to_string(logLevel) << ": ";
		(*output) << message;
		(*output) << logException(e, 1);
		(*output) << endl;
		(*output).flush();
	}
}

void Logger::error(const char* message, const exception& e) {
	LOGGER_FUNCTION_EXCEPTION(LogLevel::ERROR);
}

void Logger::warning(const char* message, const exception& e) {
	LOGGER_FUNCTION_EXCEPTION(LogLevel::WARNING);
}

void Logger::info(const char* message, const exception& e) {
	LOGGER_FUNCTION_EXCEPTION(LogLevel::INFO);
}

void Logger::debug(const char* message, const exception& e) {
	LOGGER_FUNCTION_EXCEPTION(LogLevel::DEBUG);
}

void Logger::trace(const char* message, const exception& e) {
	LOGGER_FUNCTION_EXCEPTION(LogLevel::TRACE);
}

void Logger::error(const char* format, ...)  {
	LOGGER_FUNCTION_VA(LogLevel::ERROR);
}

void Logger::warning(const char* format, ...)  {
	LOGGER_FUNCTION_VA(LogLevel::WARNING);
}

void Logger::info(const char* format, ...)  {
	LOGGER_FUNCTION_VA(LogLevel::INFO);
}

void Logger::debug(const char* format, ...)  {
	LOGGER_FUNCTION_VA(LogLevel::DEBUG);
}

void Logger::trace(const char* format, ...) {
	LOGGER_FUNCTION_VA(LogLevel::TRACE);
}

