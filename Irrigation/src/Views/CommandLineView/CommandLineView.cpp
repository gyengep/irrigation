/*
 * CommandLineView.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "CommandLineView.h"
#include "Commands.h"

#include <array>
#include <cstring>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"


#define PRINT_RUNTIME(id, runTime)												\
	std::cout << id << " - " << runTime << " min" << std::endl;

#define PRINT_RUNTIMES(runTimes)												\
{																				\
	std::cout << "Run times:" << std::endl;										\
	for (auto it = runTimes.cbegin(); runTimes.cend() != it; ++it) {			\
		PRINT_RUNTIME(it->first, it->second);									\
	}																			\
}

#define PRINT_STARTTIME(id, startTime)											\
	std::cout << id << " - " << startTime << " min" << std::endl;

#define PRINT_STARTTIMES(startTimes)											\
{																				\
	std::cout << "Start times:" << std::endl;									\
	for (auto it = startTimes.cbegin(); startTimes.cend() != it; ++it) {		\
		PRINT_STARTTIME(it->first, it->second);									\
	}																			\
}


CommandLineView::CommandLineView() :
	isTerminated(false)
{
	commandExecutor.addCommand(new command::Help(this, getDocument()));
	commandExecutor.addCommand(new command::Exit(this, getDocument()));

	commandExecutor.addCommand(new command::ProgramList(this, getDocument()));
	commandExecutor.addCommand(new command::ProgramShow(this, getDocument()));
	commandExecutor.addCommand(new command::ProgramAdd(this, getDocument()));
	commandExecutor.addCommand(new command::ProgramDelete(this, getDocument()));
	commandExecutor.addCommand(new command::ProgramRename(this, getDocument()));
	commandExecutor.addCommand(new command::ProgramMove(this, getDocument()));

	commandExecutor.addCommand(new command::StarttimeList(this, getDocument()));
	commandExecutor.addCommand(new command::StarttimeAdd(this, getDocument()));
	commandExecutor.addCommand(new command::StarttimeDelete(this, getDocument()));
	commandExecutor.addCommand(new command::StarttimeSet(this, getDocument()));
	commandExecutor.addCommand(new command::StarttimeGet(this, getDocument()));

	commandExecutor.addCommand(new command::RuntimeList(this, getDocument()));
	commandExecutor.addCommand(new command::RuntimeSet(this, getDocument()));
	commandExecutor.addCommand(new command::RuntimeGet(this, getDocument()));

	commandExecutor.addCommand(new command::Valve(this, getDocument()));
	commandExecutor.addCommand(new command::Zone(this, getDocument()));
	commandExecutor.addCommand(new command::ResetValves(this, getDocument()));

	workerThread = new std::thread(&CommandLineView::workerFunc, this);
}

CommandLineView::~CommandLineView() {
	isTerminated = true;

	workerThread->join();
	delete workerThread;
	workerThread = NULL;
}

IrrigationDocument* CommandLineView::getDocument() {
	return dynamic_cast<IrrigationDocument*>(View::getDocument());
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

void CommandLineView::workerFunc() {
	Tokens tokens;

	while (!isTerminated) {
		std::string line;

		std::cout << "> ";
		std::getline(std::cin, line);
		tokenize(line, tokens);

		try {
			commandExecutor.execute(tokens);
		} catch (const IrrigationException& e) {
			std::cout << "Error: " << e.what() << std::endl;
		} catch (std::exception& e) {
			std::cout << "Exception occured: " << e.what() << std::endl;
		}

		std::cout << std::endl;
	}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/*
	initscr();

	while(!isTerminated) {

		char ch = getch();

		if (255 != ch) {
			if (10 == ch) {
				printf("%s\r\n", text.c_str());
				if (text == "exit") {
					isTerminated = true;
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

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/*
int main(void) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    // Watch stdin (fd 0) to see when it has input.
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    // Wait up to five seconds.
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    retval = select(1, &rfds, NULL, NULL, &tv);
    // Don’t rely on the value of tv now!

    if (retval == -1)
        perror("select()");
    else if (retval)
        printf("Data is available now.\n");
        // FD_ISSET(0, &rfds) will be true.
    else
        printf("No data within five seconds.\n");
    return 0;
} */

/*
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

						FuncType f = commands[i].func;
						(this->*f)(subcommand, tokens);
					} catch (CommandLineException& e) {
						std::cerr << "Error: " << e.what() << std::endl;
					} catch (std::exception& e) {
						std::cerr << "Exception occured: " << e.what() << std::endl;
					}
					break;
				}
			}

			if (commands[i].cmd == NULL) {
				std::cerr << "Unknown command - '" << tokens.front() << "'" << std::endl;
			}

			std::cout << std::endl;
		}
*/
}

void CommandLineView::onHelpSuccess() {
	std::cout << "exit" << std::endl;
	std::cout << "help" << std::endl;

	std::cout << "program list" << std::endl;
	std::cout << "program show <programID>" << std::endl;
	std::cout << "program add <name>" << std::endl;
	std::cout << "program delete <programID>" << std::endl;
	std::cout << "program rename <programID> <name>" << std::endl;
	std::cout << "program move <programID> <newPosition>" << std::endl;

	std::cout << "reset valves" << std::endl;

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

	std::cout << "startprog <programID>" << std::endl;
	std::cout << "startzone <programID> <zoneID>" << std::endl;
	std::cout << "stop" << std::endl;

	std::cout << "time get" << std::endl;
	std::cout << "time set" << std::endl;
}

void CommandLineView::onStarttimeListSuccess(const StartTimeContainer& startTimes) {
	//PRINT_STARTTIMES(startTimes.container());
}

void CommandLineView::onStarttimeSetSuccess() {
}

void CommandLineView::onStarttimeGetSuccess(IdType startTimeId, unsigned startTime) {
	//PRINT_STARTTIME(startTimeId, startTime);
}

void CommandLineView::onStarttimeAddSuccess() {
}

void CommandLineView::onStarttimeDeleteSuccess() {
}

void CommandLineView::onRuntimeListSuccess(const RunTimeContainer& runTimes) {
	//PRINT_RUNTIMES(runTimes.container());
}

void CommandLineView::onRuntimeSetSuccess() {
}

void CommandLineView::onRuntimeGetSuccess(IdType runTimeId, unsigned runTime) {
//	PRINT_RUNTIME(runTimeId, runTime);
}

void CommandLineView::onProgramListSuccess(const ProgramContainer& programs) {
/*
	std::cout << "Programs:" << std::endl;
	for (auto it = programs.cbegin(); programs.cend() != it; ++it) {
		IdType id = it->first;
		const Program* program = it->second;
		std::cout << id << " - " << program->getName() << std::endl;
	}
*/
}

void CommandLineView::onProgramShowSuccess(const Program& program) {
	std::cout << "Name: " << program.getName() << std::endl;

	//PRINT_RUNTIMES(program.runTimes().container());
	//PRINT_STARTTIMES(program.startTimes().container());
}

void CommandLineView::onProgramAddSuccess() {
}

void CommandLineView::onProgramDeleteSuccess() {
}

void CommandLineView::onProgramRenameSuccess() {
}

void CommandLineView::onProgramMoveSuccess() {
}

void CommandLineView::onValveSuccess() {
}

void CommandLineView::onZoneSuccess() {
}

void CommandLineView::onResetValvesSuccess() {
}

void CommandLineView::onExitSuccess() {
}
