/*
 * Logger.cpp
 *
 *  Created on: 2016. okt. 10.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Logger.h"

#include <cstdio>
#include <thread>


#define LOGGER_FUNCTION(LEVEL)		\
	if (LEVEL <= level) {			\
		va_list args;				\
		va_start(args, format);		\
		log(LEVEL, format, args);	\
		va_end(args);				\
	}


std::atomic<Logger*> Logger::instance(nullptr);
std::mutex Logger::initMutex;


Logger& Logger::getInstance() {
	if (nullptr == instance) {
		std::lock_guard<std::mutex> lock(initMutex);

		if (nullptr == instance) {
			instance = new Logger();
		}
	}

	return *instance;
}

Logger::Logger() :
	logMutex(),
	level(ERROR),
	logFile()
{
	buffer[bufferSize] = '\0';
}

Logger::~Logger() {
	if (logFile.is_open()) {
		logFile.close();
	}
}

void Logger::setFile(const char* fileName /*= NULL*/) {
	if (logFile.is_open()) {
		logFile.close();
	}

	if (NULL != fileName) {
		logFile.open(fileName, std::ios::out | std::ios::app);

		if (!logFile.is_open()) {
			std::cerr << "Logfile create error!" << std::endl;
		}
	}
}

void Logger::setLevel(Level level) {
	this->level = level;
}

const char* Logger::getLevelText(Level level) {
	const static char* levelTexts[] = {
		"ERROR",
		"WARNING",
		"INFO",
		"DEBUG",
		"TRACE"
	};

	return levelTexts[level];
}

void Logger::log(Level level, const char * format, va_list args) {

	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);

	std::lock_guard<std::mutex> lock(logMutex);

	std::ostream& logOutput = (logFile.is_open() ? logFile : std::cout);

	strftime(buffer, bufferSize, "%Y.%m.%d %H:%M:%S", timeinfo);
	logOutput << buffer << " ";

	logOutput << std::this_thread::get_id() << " ";

	logOutput << getLevelText(level) << ": ";

	vsnprintf(buffer, bufferSize, format, args);
	logOutput << buffer << std::endl;

	logOutput.flush();
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

