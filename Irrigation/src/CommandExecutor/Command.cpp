#include "Command.h"
#include <stdexcept>


Command::Command(const char* command, const char* comment) :
	command(command ? command : ""),
	comment(comment ? comment : "")
{
	if (this->command.empty()) {
		throw std::invalid_argument("Command(): invalid command");
	}
}

Command::~Command() {
}
