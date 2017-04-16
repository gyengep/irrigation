#include "common.h"
#include "CommandLineView.h"
#include "Commands.h"

#include <array>
#include <cstring>
#include <list>
#include <utility>
#include <vector>

#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"
#include "Utils/Stdin.h"


CommandLineView::CommandLineView() :
	workerThread(NULL),
	isTerminated(false)
{
}

CommandLineView::~CommandLineView() {
	isTerminated = true;

	if (workerThread) {
		workerThread->join();
		delete workerThread;
		workerThread = NULL;
	}
}

IrrigationDocument* CommandLineView::getDocument() {
	return dynamic_cast<IrrigationDocument*>(View::getDocument());
}

void CommandLineView::init() {
	LOGGER.debug("CommandLineView started");

	commandExecutor.addCommand(new CommandLine::ProgramList(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramShow(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramAdd(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramDelete(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramRename(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramMove(getDocument()));
	commandExecutor.addCommand(new CommandLine::SpaceHolder());

	commandExecutor.addCommand(new CommandLine::StarttimeList(getDocument()));
	commandExecutor.addCommand(new CommandLine::StarttimeAdd(getDocument()));
	commandExecutor.addCommand(new CommandLine::StarttimeDelete(getDocument()));
	commandExecutor.addCommand(new CommandLine::StarttimeSet(getDocument()));
	commandExecutor.addCommand(new CommandLine::StarttimeGet(getDocument()));
	commandExecutor.addCommand(new CommandLine::SpaceHolder());

	commandExecutor.addCommand(new CommandLine::RuntimeList(getDocument()));
	commandExecutor.addCommand(new CommandLine::RuntimeSet(getDocument()));
	commandExecutor.addCommand(new CommandLine::RuntimeGet(getDocument()));
	commandExecutor.addCommand(new CommandLine::SpaceHolder());

	commandExecutor.addCommand(new CommandLine::Valve(getDocument()));
	commandExecutor.addCommand(new CommandLine::Zone(getDocument()));
	commandExecutor.addCommand(new CommandLine::ResetValves(getDocument()));
	commandExecutor.addCommand(new CommandLine::SpaceHolder());

	commandExecutor.addCommand(new CommandLine::Exit(getDocument()));
	commandExecutor.addCommand(new CommandLine::Help(this, getDocument()));

	workerThread = new std::thread(&CommandLineView::workerFunc, this);
}

void CommandLineView::terminate() {
	LOGGER.debug("CommandLineView stopped");
	isTerminated = true;
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

	std::cout << "> " << std::flush;

	while (!isTerminated) {

		const char* line = Stdin::getInstance().readLine();

		if (NULL != line) {
			Tokens tokens;
			tokenize(std::string(line), tokens);

			try {
				commandExecutor.execute(tokens);
			} catch (const IrrigationException& e) {
				std::cout << "Error: " << e.what() << std::endl;
			} catch (const std::exception& e) {
				std::cout << "Exception occured: " << e.what() << std::endl;
			} catch (...) {
				std::cout << "Unknown error!" << std::endl;
			}

			std::cout << std::endl;
			std::cout << "> " << std::flush;
		}
	}
}




