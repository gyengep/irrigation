#pragma once
#include <gmock/gmock.h>
#include "Command/CommandExecutor.h"


class CommandTest : public Command {
public:

	CommandTest(const char* command) : Command(NULL, command, "") {
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