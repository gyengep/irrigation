#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "Command/CommandExecutor.h"
#include "DocumentView/View.h"

#include <memory>

class CommandLineView : public View {
	std::thread* workerThread;
	std::atomic_bool isTerminated;
	CommandExecutor commandExecutor;

	void workerFunc();

public:
	CommandLineView();
	virtual ~CommandLineView();

	virtual void init();
	virtual void terminate();

	IrrigationDocument* getDocument();
	const CommandExecutor& getCommandExecutor() const { return commandExecutor; }

	static void tokenize(const std::string& text, Tokens& tokens);
};
