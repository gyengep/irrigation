/*
 * Logger.h
 *
 *  Created on: 2016. okt. 10.
 *      Author: Rendszergazda
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <mutex>
#include <stdarg.h>

#define LOGGER()		Logger::getInstance()
#define LOG_ERROR 		Logger::getInstance().error
#define LOG_WARNING 	Logger::getInstance().warning
#define LOG_INFO	 	Logger::getInstance().info
#define LOG_DEBUG	 	Logger::getInstance().debug
#define LOG_TRACE	 	Logger::getInstance().trace

class Logger {
public:

	enum Level {
		ERROR,
		WARNING,
		INFO,
		DEBUG,
		TRACE
	};

private:
	static Logger* instance;
	static std::mutex initMutex;
	static const unsigned bufferSize = 1000;

	std::mutex logMutex;
	char buffer[bufferSize + 1];
	Level level;
	//std::ofstream out;

	Logger();
	void log(Level level, const char * format, va_list args);

public:
	virtual ~Logger();
	static Logger& getInstance();

	void setLevel(Level level);
	//void setFile(const char* fileName);

	void error(const char * format, ...);
	void warning(const char * format, ...);
	void info(const char * format, ...);
	void debug(const char * format, ...);
	void trace(const char * format, ...);
};

#endif /* LOGGER_H_ */
