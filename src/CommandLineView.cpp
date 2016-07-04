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

CommandLineView::CommandLineView(Document* document) : View(document), isTerminated(false) {
	workerThread = new std::thread(workerFunc, this);
}

CommandLineView::~CommandLineView() {
	isTerminated = true;
	
	workerThread->join();
	delete workerThread;
	workerThread = NULL;
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
	// { "runtime", CommandLineView::cmd_runtime},
	// { "starttime", CommandLineView::cmd_starttime},
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
	std::cout << "program add <name>" << std::endl;
	std::cout << "program delete <programID>" << std::endl;
	std::cout << "program rename <programID> <name>" << std::endl;
	std::cout << "program move <programID> <newIndex>" << std::endl;

	std::cout << "runtime set <programID> <runtimeIdx> <runtime>" << std::endl;
	std::cout << "runtime get <programID> <runtimeIdx>" << std::endl;

	std::cout << "startime add <programID> <starttime>" << std::endl;
	std::cout << "startime delete <programID> <starttimeIdx>" << std::endl;
	std::cout << "startime set <programID> <starttimeIdx> <starttime>" << std::endl;
	std::cout << "startime get <programID> <starttimeIdx>" << std::endl;

	std::cout << "valve <valveID> {on|off}" << std::endl;
	std::cout << "zone <zoneID> {on|off}" << std::endl;
	std::cout << "time get" << std::endl;
	std::cout << "time set" << std::endl;
}

void CommandLineView::cmd_program(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {

	Document* document = p->getDocument();

	if (subcommand == "list") {
/*
		CHECK_PARAMETERS(0);

		const Document::ProgramList& programs = document->getPrograms();

		std::cout << "Programs:" << std::endl;
		for (auto it = programs.begin(); programs.end() != it; ++it) {
			const Program* program = *it;
			std::cout << program->getID() << " - " << program->getName() << std::endl;
		}

		document->releasePrograms();
*/
	} else if (subcommand == "add") {
		CHECK_PARAMETERS(1);

		Program& program = document->addProgram();
		program.setName(parameters[0]);

	} else if (subcommand == "delete") {
		CHECK_PARAMETERS(1);

		unsigned long programId = parseUInt(parameters[0], "Invalid programID");
		document->deleteProgram(programId);

	} else if (subcommand == "rename") {
		CHECK_PARAMETERS(2);

		unsigned long programId = parseUInt(parameters[0], "Invalid programID");
		document->getProgram(programId).setName(parameters[1]);

	} else if (subcommand == "move") {
		CHECK_PARAMETERS(2);

		unsigned long programId = parseUInt(parameters[0], "Invalid programID");
		unsigned long newProgramIdx = parseUInt(parameters[1], "Invalid index");
		document->moveProgram(programId, newProgramIdx);

	} else {
		throw UnknownSubcommandException();
	}
}

void CommandLineView::cmd_valve(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
	CHECK_PARAMETERS(2);

	Document* document = p->getDocument();

	unsigned valveId = parseUInt(parameters[0], "Invalid valveID");
	bool open = parseOnOff(parameters[1], "Invalid parameter");
	document->openValve(valveId, open);
}

void CommandLineView::cmd_zone(CommandLineView* p, const std::string& subcommand, const Tokens& parameters) {
/*
	unsigned long zone;
	bool open;

	if (args.size() != 2) {
		throw CommandLineException("Keves parameter");
	}

	if (!parseUInt(args[0], zone)) {
		throw CommandLineException("Invalid zoneIdx");
	}

	if (p->getDocument()->getZoneCount() <= zone) {
		throw CommandLineException("Invalid zoneIdx 2");
	}

	if (!parseOnOff(args[1], open)) {
		throw CommandLineException("{on|off}");
	}

	p->getDocument()->openZone((unsigned)zone, open);
*/
}
