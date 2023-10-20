#pragma once
#include <list>
#include <string>


class LogEntryDto {
	const std::string time;
	const std::string thread;
	const std::string level;
	const std::string text;

public:
	LogEntryDto(const std::string& time, const std::string& thread, const std::string& level, const std::string& text) :
		time(time),
		thread(thread),
		level(level),
		text(text)
	{
	}

	const std::string& getTime() const { return time; }
	const std::string& getThread() const { return thread; }
	const std::string& getLevel() const { return level; }
	const std::string& getText() const { return text; }
};

typedef std::list<LogEntryDto> LogEntryDtoList;
