#pragma once
#include <gmock/gmock.h>
#include "CommandExecutor/Command.h"
#include "CommandExecutor/CommandExecutor.h"


class CommandTest : public Command {
public:

	CommandTest(const char* command) : Command(command, "") {
	}

	MOCK_METHOD1(execute, void(const Tokens& parameters));
};


class CommandExecutorTest : public testing::Test {
protected:
	CommandExecutor commandExecutor;
	CommandTest* command1;
	CommandTest* command2;

public:
	virtual void SetUp();
	virtual void TearDown();
};
