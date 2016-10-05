/*
 * CommandLineView.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef COMMANDLINEVIEW_H_
#define COMMANDLINEVIEW_H_

#include <atomic>
#include <string>
#include <thread>

#include "CommandExecutor.h"
#include "View.h"


class CommandLineView : public CommandExecutor, View {
	std::thread* workerThread;
	std::atomic_bool isTerminated;

	void workerFunc();

	virtual void onExecutionFailed(const CommandLineException& e);
	virtual void onError(const std::exception& e);

public:
	CommandLineView(Document* document);
	virtual ~CommandLineView();

	static void tokenize(const std::string& text, Tokens& tokens);
};

#endif /* COMMANDLINEVIEW_H_ */
