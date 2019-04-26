#pragma once
#include <array>
#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <ostream>
#include <stdarg.h>
#include <stdexcept>
#include <string>


#define LOGGER Logger::getInstance()


enum class LogLevel {
	OFF,
	ERROR,
	WARNING,
	INFO,
	DEBUG,
	TRACE
};

std::string to_string(LogLevel logLevel);


class Logger {
public:

	struct Entry {
		const std::string time;
		const std::string thread;
		const std::string level;
		const std::string text;

		Entry(const std::string time,
				const std::string thread,
				const std::string level,
				const std::string text);
	};

private:
	mutable std::mutex mtx;
	std::atomic<LogLevel> logLevel;
	std::shared_ptr<std::ostream> output;
	std::deque<Entry> logEntries;

	Logger();

	void log(LogLevel logLevel, const char* message, const std::exception* e);

	static std::string getTime();
	static std::string getThread();
	static std::string logException(const std::exception* e, unsigned level);

public:
	virtual ~Logger();
	static Logger& getInstance();

	LogLevel getLevel() const { return logLevel; }
	void setLevel(LogLevel logLevel);
	void setOutputStream(std::shared_ptr<std::ostream> output);
	void setFileName(std::string fileName);
	bool isLoggable(LogLevel logLevel) const;

	void error(const char* message, const std::exception& e);
	void warning(const char* message, const std::exception& e);
	void info(const char* message, const std::exception& e);
	void debug(const char* message, const std::exception& e);
	void trace(const char* message, const std::exception& e);

	void error(const char* format, ...);
	void warning(const char* format, ...);
	void info(const char* format, ...);
	void debug(const char* format, ...);
	void trace(const char* format, ...);

	std::deque<Entry> getEntries() const;
};
