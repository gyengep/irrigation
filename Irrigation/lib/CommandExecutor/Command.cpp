#include "Command.h"
#include <stdexcept>

using namespace std;


Command::Command(const char* command, const char* comment) :
	command(command ? command : ""),
	comment(comment ? comment : "")
{
	if (this->command.empty()) {
		throw invalid_argument("Command(): invalid command");
	}
}

Command::~Command() {
}
