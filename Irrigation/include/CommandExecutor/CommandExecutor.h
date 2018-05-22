#pragma once
#include <list>
#include <mutex>
#include <string>
#include <vector>


class Command;
typedef std::vector<std::string> Tokens;


class CommandExecutor {
public:

	typedef std::list<Command*> Commands;

private:
	Commands commands;
	mutable std::mutex mutex;

	Command* getMatchingCommand(const std::string& command) const;

public:
	CommandExecutor();
	virtual ~CommandExecutor();

	void addCommand(Command* command);
	void execute(const Tokens& tokens) const;

	const Commands& getCommands() const { return commands; }
};
