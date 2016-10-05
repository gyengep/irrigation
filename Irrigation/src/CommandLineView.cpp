/*
 * CommandLineView.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "CommandLineView.h"

#include <cstring>
#include <sstream>

#include "Application.h"
#include "Commands.h"
#include "Document.h"
#include "Program.h"


CommandLineView::CommandLineView(Document* document) :
	CommandExecutor(),
	View(document),
	isTerminated(false)
{
	addCommand(new command::Help(getDocument()));
	addCommand(new command::Exit(getDocument()));

	addCommand(new command::ProgramList(getDocument()));
	addCommand(new command::ProgramShow(getDocument()));
	addCommand(new command::ProgramAdd(getDocument()));
	addCommand(new command::ProgramDelete(getDocument()));
	addCommand(new command::ProgramRename(getDocument()));
	addCommand(new command::ProgramMove(getDocument()));

	addCommand(new command::StarttimeList(getDocument()));
	addCommand(new command::StarttimeAdd(getDocument()));
	addCommand(new command::StarttimeDelete(getDocument()));
	addCommand(new command::StarttimeSet(getDocument()));
	addCommand(new command::StarttimeGet(getDocument()));

	addCommand(new command::RuntimeList(getDocument()));
	addCommand(new command::RuntimeSet(getDocument()));
	addCommand(new command::RuntimeGet(getDocument()));

	addCommand(new command::Valve(getDocument()));
	addCommand(new command::Zone(getDocument()));
	addCommand(new command::ResetValves(getDocument()));

	workerThread = new std::thread(&CommandLineView::workerFunc, this);
}

CommandLineView::~CommandLineView() {
	isTerminated = true;

	workerThread->join();
	delete workerThread;
	workerThread = NULL;
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

		execute(tokens);
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


void CommandLineView::onExecutionFailed(const CommandLineException& e) {
	std::cout << "Error: " << e.what() << std::endl;
}

void CommandLineView::onError(const std::exception& e) {
	std::cout << "Exception occured: " << e.what() << std::endl;
}
