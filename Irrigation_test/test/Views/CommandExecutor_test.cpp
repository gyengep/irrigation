
#include "Common.h"
#include "Command/CommandExecutor.h"


class CommandTest : public Command {
	Tokens tokens;
	int executeCounter;

public:

	CommandTest(const char* command) :
		Command(NULL, command),
		executeCounter(0)
	{
	}

	virtual void execute(const Tokens& parameters) {
		tokens = parameters;
		executeCounter++;
	}

	const Tokens& getTokens() const { return tokens; }
	int getExecuteCounter() const { return executeCounter; }
};


class CommandExecutorTest : public testing::Test {
protected:
	CommandExecutor commandExecutor;
	CommandTest* command1;
	CommandTest* command2;

public:
	virtual void SetUp() {
		command1 = new CommandTest("command1");
		command2 = new CommandTest("command2");

		commandExecutor.addCommand(command1);
		commandExecutor.addCommand(command2);
	}

	virtual void TearDown() {
	}
};

TEST_F(CommandExecutorTest, executeSuccess1) {
	Tokens tokens;
	tokens.push_back("command1");

	commandExecutor.execute(tokens);

	EXPECT_EQ(1, command1->getExecuteCounter());
	EXPECT_EQ(0, command2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, executeSuccess2) {
	Tokens tokens;
	tokens.push_back("command2");

	commandExecutor.execute(tokens);

	EXPECT_EQ(0, command1->getExecuteCounter());
	EXPECT_EQ(1, command2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, parameters) {
	Tokens tokens;
	tokens.push_back("command1");
	tokens.push_back("P1");

	commandExecutor.execute(tokens);

	EXPECT_EQ(1, command1->getExecuteCounter());
	//EXPECT_EQ(1, command1->getTokens().size());
	//EXPECT_STREQ("P1", command1->getTokens().front().c_str());
}


TEST_F(CommandExecutorTest, unknownCommand) {
	Tokens tokens;

	tokens.push_back("unknown_command");
	EXPECT_THROW(commandExecutor.execute(tokens), UnknownCommandException);

	tokens.push_back("P1");
	EXPECT_THROW(commandExecutor.execute(tokens), UnknownCommandException);

	tokens.push_back("P2");
	EXPECT_THROW(commandExecutor.execute(tokens), UnknownCommandException);
}
