/*
 * Logger.cpp
 *
 *  Created on: 2016. okt. 10.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Logger.h"

#include <cstdio>

Logger* Logger::instance = NULL;
std::mutex Logger::initMutex;

Logger::Logger() :
	out(),
	level(ERROR)
{
}

Logger::~Logger() {
	out.close();
}

void Logger::setFile(const char* fileName) {
	std::ofstream tmp;
	tmp.open(fileName, std::ios::out | std::ios::app);

	if (tmp.is_open()) {
		out.swap(tmp);
	} else {
		std::cerr << "Logfile create error!" << std::endl;
	}
}

void Logger::setLevel(Level level) {
	std::lock_guard<std::mutex> lock(logMutex);
	this->level = level;
}

Logger& Logger::getInstance() {
	if (NULL == instance) {
		std::lock_guard<std::mutex> lock(initMutex);
		if (NULL == instance) {
			instance = new Logger();
		}
	}

	return *instance;
}

void Logger::log(Level level, const char * format, va_list args) {
	static const unsigned size = 1000;
	static char buffer[size + 1];

	std::lock_guard<std::mutex> lock(logMutex);

	if (level <= this->level) {
		buffer[size] = '\0';
		vsnprintf(buffer, size, format, args);
		std::cout << buffer << std::endl;
	}
}

void Logger::error(const char * format, ...)  {
	va_list args;
	va_start(args, format);
	log(ERROR, format, args);
	va_end(args);
}

void Logger::warning(const char * format, ...)  {
	va_list args;
	va_start(args, format);
	log(WARNING, format, args);
	va_end(args);
}

void Logger::info(const char * format, ...)  {
	va_list args;
	va_start(args, format);
	log(INFO, format, args);
	va_end(args);
}

void Logger::debug(const char * format, ...)  {
	va_list args;
	va_start(args, format);
	log(DEBUG, format, args);
	va_end(args);
}

void Logger::trace(const char * format, ...) {
	va_list args;
	va_start(args, format);
	log(TRACE, format, args);
	va_end(args);
}
