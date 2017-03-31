#pragma once

#include <atomic>
#include <string>
#include <thread>

#include "Command/CommandExecutor.h"
#include "DocumentView/View.h"


class CommandLineView : public View {

	class ReadStdin {
		fd_set rfds;
		timeval tv;
		char* buffer;
		size_t bufferSize;

	public:
		ReadStdin();
		~ReadStdin();
		const char* readLine();
	};


	std::thread* workerThread;
	std::atomic_bool isTerminated;
	CommandExecutor commandExecutor;
	ReadStdin readStdin;

	void workerFunc();

public:
	CommandLineView();
	virtual ~CommandLineView();

	virtual void init();
	virtual void terminate();

	IrrigationDocument* getDocument();

	static void tokenize(const std::string& text, Tokens& tokens);
};
