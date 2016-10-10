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

	std::ofstream out;
	Level level;
	std::mutex logMutex;

	Logger();
	void log(Level level, const char * format, va_list args);

public:
	virtual ~Logger();
	static Logger& getInstance();

	void setLevel(Level level);
	void setFile(const char* fileName);

	void error(const char * format, ...);
	void warning(const char * format, ...);
	void info(const char * format, ...);
	void debug(const char * format, ...);
	void trace(const char * format, ...);
};

#endif /* LOGGER_H_ */
