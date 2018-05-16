#pragma once
#include <string>
#include <vector>

typedef std::vector<std::string> Tokens;


class Command {
	const std::string command;
	const std::string comment;

public:
	Command(const char* command, const char* comment);
	virtual ~Command();

	virtual void execute(const Tokens& parameters) = 0;

	const std::string& getCommand() const { return command; }
	const std::string& getComment() const { return comment; }
};
