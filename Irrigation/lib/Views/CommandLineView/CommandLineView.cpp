#include "CommandLineView.h"
#include "Commands.h"
#include "Stdin.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"


CommandLineView::CommandLineView() :
	workerThread(NULL),
	isTerminated(false)
{
}

CommandLineView::~CommandLineView() {
	isTerminated = true;

	if (workerThread) {
		LOGGER.debug("Waiting for commandLine to finish...");
		workerThread->join();
		delete workerThread;
		workerThread = NULL;
		LOGGER.debug("CommandLine finished");
	}
}

IrrigationDocument* CommandLineView::getDocument() {
	return dynamic_cast<IrrigationDocument*>(View::getDocument());
}

void CommandLineView::init() {
	LOGGER.debug("CommandLineView init called");

	commandExecutor.addCommand(new CommandLine::ProgramList(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramShow(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramAdd(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramDelete(getDocument()));
	commandExecutor.addCommand(new CommandLine::ProgramRename(getDocument()));
	commandExecutor.addCommand(new CommandLine::StartTimeList(getDocument()));
	commandExecutor.addCommand(new CommandLine::StartTimeAdd(getDocument()));
	commandExecutor.addCommand(new CommandLine::StartTimeDelete(getDocument()));
	commandExecutor.addCommand(new CommandLine::StartTimeSet(getDocument()));
	commandExecutor.addCommand(new CommandLine::RunTimeList(getDocument()));
	commandExecutor.addCommand(new CommandLine::RunTimeSet(getDocument()));
	commandExecutor.addCommand(new CommandLine::Valve(getDocument()));
	commandExecutor.addCommand(new CommandLine::Zone(getDocument()));
	commandExecutor.addCommand(new CommandLine::ResetValves(getDocument()));
	commandExecutor.addCommand(new CommandLine::Exit(getDocument()));
	commandExecutor.addCommand(new CommandLine::Help(getCommandExecutor(), getDocument()));
	workerThread = new std::thread(&CommandLineView::workerFunc, this);

	LOGGER.debug("CommandLineView init executed");
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
			} catch (const std::exception& e) {
				LOGGER.warning(e.what());
				std::cout << "Error: " << e.what() << std::endl;
			} catch (...) {
				LOGGER.warning("CommandLineView Unknown error!");
				std::cout << "Unknown error!" << std::endl;
			}

			std::cout << std::endl;
			std::cout << "> " << std::flush;
		}
	}
}

void CommandLineView::terminate() {
	LOGGER.debug("CommandLineView terminate called");
	isTerminated = true;
	LOGGER.debug("CommandLineView terminate executed");
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
