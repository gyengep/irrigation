/*
 * CommandLineView.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "CommandLineView.h"

#include <climits>
#include <thread>
#include <sstream>
#include <cstring>
#include <stdexcept>

#include "Application.h"
#include "Document.h"
#include "Program.h"


#define	CHECK_SUBCOMMAND()					\
if (subcommand.empty()) { 					\
	throw SubCommandMissingException(); 	\
}

#define CHECK_PARAMETERS(req)				\
if (parameters.size() < req) {				\
	throw TooFewArgumentsException();		\
} else if (parameters.size() > req) {		\
	throw TooMuchArgumentsException();		\
}


#define PRINT_RUNTIME(id, runTime)											\
	std::cout << id << " - " << runTime << " min" << std::endl;

#define PRINT_RUNTIMES(program)												\
{																			\
	const Program::RunTimes& runTimes = program.getRunTimes();				\
	std::cout << "Run times:" << std::endl;									\
	for (auto it = runTimes.cbegin(); runTimes.cend() != it; ++it) {		\
		PRINT_RUNTIME(it->first, it->second);								\
	}																		\
	program.releaseRunTimes();												\
}

#define PRINT_STARTTIME(id, startTime)										\
	std::cout << id << " - " << startTime << " min" << std::endl;

#define PRINT_STARTTIMES(program)											\
{																			\
	const Program::StartTimes& startTimes = program.getStartTimes();		\
	std::cout << "Start times:" << std::endl;								\
	for (auto it = startTimes.cbegin(); startTimes.cend() != it; ++it) {	\
		PRINT_STARTTIME(it->first, it->second);								\
	}																		\
	program.releaseStartTimes();											\
}



CommandLineView::CommandLineView(Document* document) : View(document), isTerminated(false) {
	workerThread = new std::thread(workerFunc, this);
}

CommandLineView::~CommandLineView() {
	isTerminated = true;
	
	workerThread->join();
	delete workerThread;
	workerThread = NULL;
}

IdType CommandLineView::parseId(const std::string& text, const char* errorMessage) {
	return (IdType)parseUInt(text, errorMessage);
}

unsigned CommandLineView::parseUInt(const std::string& text, const char* errorMessage) {
	std::size_t pos;
	unsigned long result =std::stoul(text, &pos, 10);
	if (text.length() != pos || result > UINT_MAX) {
		throw CommandLineException(errorMessage);
	}
	return result;
}

bool CommandLineView::parseOnOff(const std::string& text, const char* errorMessage) {
	bool result;

	if (text == "on") {
		result = true;
	} else if (text == "off") {
		result = false;
	} else {
		throw CommandLineException(errorMessage);
	}

	return result;
}

void CommandLineView::tokenize(const std::string& text, Tokens& tokens) {
	tokens.clear();

	const char* delimeters = " \t\r\n";
	char* text_copy = new char[text.length() + 1];
	memcpy(text_copy, text.c_str(), text.length() + 1);

	char* pch = strtok(text_copy, delimeters);
	while (pch != NULL) {
		tokens.push_back(pch);
		pch = strtok(NULL, delimeters);
	}

	delete[] text_copy;
}

CommandLineView::Commands CommandLineView::commands[] = {
	// { "exit", CommandLineView::cmd_exit },
	{ "help", false, CommandLineView::cmd_help },
	{ "program", true, CommandLineView::cmd_program},
	{ "runtime", true, CommandLineView::cmd_runtime},
	{ "starttime", true, CommandLineView::cmd_starttime},
	{ "valve", false, CommandLineView::cmd_valve },
	{ "zone", false, CommandLineView::cmd_zone },
	{ NULL, NULL }
};

void CommandLineView::workerFunc(CommandLineView* p) {
	Tokens tokens;
	std::string line;

	while (!p->isTerminated) {
		std::getline(std::cin, line);
		tokenize(line, tokens);

		std::cout << "Tokens:";
		for (unsigned i = 0; i < tokens.size(); ++i) {
			std::cout << " \"" << tokens[i] << "\"";
		}
		std::cout << std::endl;

		if (!tokens.empty()) {
			int i;
			for (i = 0; commands[i].cmd; i++) {
				if (tokens.front() == commands[i].cmd) {
					tokens.erase(tokens.begin());

					try {
						std::string subcommand;

						if (commands[i].subcommand) {
							if (tokens.empty()) {
								throw CommandLineException("Subcommand missing");
							}

							subcommand = tokens.front();
							tokens.erase(tokens.begin());
						}

						commands[i].func(p, subcommand, tokens);
					} catch (std::exception& e) {
						std::cerr << "Error: " << e.what() << std::endl;
					}
					break;
				}
			}

			if (commands[i].cmd == NULL) {
				std::cerr << "Unknown command - \"" << tokens.front() << "\"" << std::endl;
			}
		}
	}

/*
	initscr();
	
	while(!p->isTerminated) {
	
		char ch = getch();
		
		if (255 != ch) {
			if (10 == ch) {
				printf("%s\r\n", text.c_str());
				if (text == "exit") {
					p->isTerminated = true;
					break;
				}
				text.clear();
			} else {
				text += ch;
			}
		} else {
			printf("%c", ch);
			usleep(10000);
		}
	}
	
	endwin();
	getApplication()->terminate();
*/	
}
/*
void CommandLineView::cmd_exit(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
	std::cout << "cmd_exit()" << std::endl;

	if (!args.empty()) {
		// TODO exception
		std::cout << "Invalid parameter" << std::endl;
		return;
	}

	//TODO ez itt nem mukodik
	p->isTerminated = true;
}
*/
void CommandLineView::cmd_help(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
	CHECK_PARAMETERS(0);

	//std::cout << "exit" << std::endl;
	std::cout << "help" << std::endl;

	std::cout << "program list" << std::endl;
	std::cout << "program show <programID>" << std::endl;
	std::cout << "program add <name>" << std::endl;
	std::cout << "program delete <programID>" << std::endl;
	std::cout << "program rename <programID> <name>" << std::endl;
	std::cout << "program move <programID> <newPosition>" << std::endl;

	std::cout << "runtime list <programID>" << std::endl;
	std::cout << "runtime set <programID> <runtimeID> <runtime>" << std::endl;
	std::cout << "runtime get <programID> <runtimeID>" << std::endl;

	std::cout << "startime list <programID>" << std::endl;
	std::cout << "startime add <programID> <starttime>" << std::endl;
	std::cout << "startime delete <programID> <starttimeID>" << std::endl;
	std::cout << "startime set <programID> <starttimeID> <starttime>" << std::endl;
	std::cout << "startime get <programID> <starttimeID>" << std::endl;

	std::cout << "valve <valveID> {on|off}" << std::endl;
	std::cout << "zone <zoneID> {on|off}" << std::endl;

	std::cout << "time get" << std::endl;
	std::cout << "time set" << std::endl;
}

void CommandLineView::cmd_starttime(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {

	Document* document = p->getDocument();

	if (subcommand == "list") {
		CHECK_PARAMETERS(1);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		const Program& program = document->getProgram(programId);

		PRINT_STARTTIMES(program);

	} else if (subcommand == "set") {
		CHECK_PARAMETERS(3);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = parseId(parameters[1], INVALID_STARTTIMEID);
		unsigned startTime = parseUInt(parameters[2], "Invalid starttime");
		document->getProgram(programId).setStartTime(startTimeId, startTime);

	} else if (subcommand == "get") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = parseId(parameters[1], INVALID_STARTTIMEID);
		unsigned startTime = document->getProgram(programId).getStartTime(startTimeId);

		PRINT_STARTTIME(startTimeId, startTime);

	} else if (subcommand == "add") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		unsigned startTime = parseUInt(parameters[1], "Invalid starttime");
		document->getProgram(programId).addStartTime(startTime);

	} else if (subcommand == "delete") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		IdType startTimeId = parseId(parameters[1], INVALID_STARTTIMEID);
		document->getProgram(programId).deleteStartTime(startTimeId);

	} else {
		throw UnknownSubcommandException();
	}
}

void CommandLineView::cmd_runtime(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {

	Document* document = p->getDocument();

	if (subcommand == "list") {
		CHECK_PARAMETERS(1);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		const Program& program = document->getProgram(programId);

		PRINT_RUNTIMES(program);

	} else if (subcommand == "set") {
		CHECK_PARAMETERS(3);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = parseId(parameters[1], INVALID_RUNTIMEID);
		unsigned runTime = parseUInt(parameters[2], "Invalid runtime");
		document->getProgram(programId).setRunTime(runTimeId, runTime);

	} else if (subcommand == "get") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		IdType runTimeId = parseId(parameters[1], INVALID_RUNTIMEID);
		unsigned runTime = document->getProgram(programId).getRunTime(runTimeId);

		PRINT_RUNTIME(runTimeId, runTime);

	} else {
		throw UnknownSubcommandException();
	}
}

void CommandLineView::cmd_program(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {

	Document* document = p->getDocument();

	if (subcommand == "list") {
		CHECK_PARAMETERS(0);

		const Document::ProgramList& programs = document->getPrograms();

		std::cout << "Programs:" << std::endl;
		for (auto it = programs.cbegin(); programs.cend() != it; ++it) {
			IdType id = it->first;
			const Program* program = it->second;
			std::cout << id << " - " << program->getName() << std::endl;
		}

		document->releasePrograms();

	} else if (subcommand == "show") {
		CHECK_PARAMETERS(1);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		const Program& program = document->getProgram(programId);

		std::cout << "Name: " << program.getName() << std::endl;

		PRINT_RUNTIMES(program);
		PRINT_STARTTIMES(program);

	} else if (subcommand == "add") {
		CHECK_PARAMETERS(1);

		Program& program = document->addProgram();
		program.setName(parameters[0]);

	} else if (subcommand == "delete") {
		CHECK_PARAMETERS(1);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		document->deleteProgram(programId);

	} else if (subcommand == "rename") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		document->getProgram(programId).setName(parameters[1]);

	} else if (subcommand == "move") {
		CHECK_PARAMETERS(2);

		IdType programId = parseId(parameters[0], INVALID_PROGRAMID);
		unsigned position = parseUInt(parameters[1], "Invalid position");
		document->moveProgram(programId, position);

	} else {
		throw UnknownSubcommandException();
	}
}

void CommandLineView::cmd_valve(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
	CHECK_PARAMETERS(2);

	Document* document = p->getDocument();

	IdType valveId = parseId(parameters[0], INVALID_VALVEID);
	bool open = parseOnOff(parameters[1], "Invalid parameter");
	document->openValve(valveId, open);
}

void CommandLineView::cmd_zone(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
	CHECK_PARAMETERS(2);

	Document* document = p->getDocument();

	IdType zoneId = parseId(parameters[0], INVALID_ZONEID);
	bool open = parseOnOff(parameters[1], "Invalid parameter");
	document->openZone(zoneId, open);
}
