#pragma once
#include <list>
#include <string>


class LogEntryDTO {
	const std::string time;
	const std::string thread;
	const std::string level;
	const std::string text;

public:
	LogEntryDTO(const std::string& time, const std::string& thread, const std::string& level, const std::string& text) :
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

typedef std::list<LogEntryDTO> LogEntryDTOList;
