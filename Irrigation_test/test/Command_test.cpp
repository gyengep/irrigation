#include "common.h"
#include "CommandExecutor.h"


class CommandTest : public Command {
	Tokens tokens;
	int executeCounter;

public:

	CommandTest(const char* command, const char* subCommand) : Command(NULL, NULL, command, subCommand)
	{
	}

	virtual void execute(const Tokens& parameters) {
		tokens = parameters;
		executeCounter++;
	}

	const Tokens& getTokens() const { return tokens; }
	int getExecuteCounter() const { return executeCounter; }
};


class CommandExecutorTest : public CommandExecutor, public testing::Test {
protected:
	CommandTest* command1;
	CommandTest* command2;
	CommandTest* command3_subcommand1;
	CommandTest* command3_subcommand2;

public:
	virtual void SetUp() {
		command1 = new CommandTest("command1", NULL);
		command2 = new CommandTest("command2", "");
		command3_subcommand1 = new CommandTest("command3", "subcommand1");
		command3_subcommand2 = new CommandTest("command3", "subcommand2");

		addCommand(command1);
		addCommand(command2);
		addCommand(command3_subcommand1);
		addCommand(command3_subcommand2);
	}

	virtual void TearDown() {
	}
};

TEST_F(CommandExecutorTest, executeSuccess) {

	int executeCounter[4];

	{
		Tokens tokens;
		tokens.push_back("command1");

		executeCounter[0] = command1->getExecuteCounter();
		executeCounter[1] = command2->getExecuteCounter();
		executeCounter[2] = command3_subcommand1->getExecuteCounter();
		executeCounter[3] = command3_subcommand2->getExecuteCounter();

		execute(tokens);

		EXPECT_EQ(executeCounter[0] + 1, command1->getExecuteCounter());
		EXPECT_EQ(executeCounter[1], command2->getExecuteCounter());
		EXPECT_EQ(executeCounter[2], command3_subcommand1->getExecuteCounter());
		EXPECT_EQ(executeCounter[3], command3_subcommand2->getExecuteCounter());
	}

	{
		Tokens tokens;
		tokens.push_back("command2");

		executeCounter[0] = command1->getExecuteCounter();
		executeCounter[1] = command2->getExecuteCounter();
		executeCounter[2] = command3_subcommand1->getExecuteCounter();
		executeCounter[3] = command3_subcommand2->getExecuteCounter();

		execute(tokens);

		EXPECT_EQ(executeCounter[0], command1->getExecuteCounter());
		EXPECT_EQ(executeCounter[1] + 1, command2->getExecuteCounter());
		EXPECT_EQ(executeCounter[2], command3_subcommand1->getExecuteCounter());
		EXPECT_EQ(executeCounter[3], command3_subcommand2->getExecuteCounter());
	}

	{
		Tokens tokens;

		tokens.push_back("command3");
		tokens.push_back("subcommand1");

		executeCounter[0] = command1->getExecuteCounter();
		executeCounter[1] = command2->getExecuteCounter();
		executeCounter[2] = command3_subcommand1->getExecuteCounter();
		executeCounter[3] = command3_subcommand2->getExecuteCounter();

		execute(tokens);

		EXPECT_EQ(executeCounter[0], command1->getExecuteCounter());
		EXPECT_EQ(executeCounter[1], command2->getExecuteCounter());
		EXPECT_EQ(executeCounter[2] + 1, command3_subcommand1->getExecuteCounter());
		EXPECT_EQ(executeCounter[3], command3_subcommand2->getExecuteCounter());
	}

	{
		Tokens tokens;
		tokens.push_back("command3");
		tokens.push_back("subcommand2");

		executeCounter[0] = command1->getExecuteCounter();
		executeCounter[1] = command2->getExecuteCounter();
		executeCounter[2] = command3_subcommand1->getExecuteCounter();
		executeCounter[3] = command3_subcommand2->getExecuteCounter();

		execute(tokens);

		EXPECT_EQ(executeCounter[0], command1->getExecuteCounter());
		EXPECT_EQ(executeCounter[1], command2->getExecuteCounter());
		EXPECT_EQ(executeCounter[2], command3_subcommand1->getExecuteCounter());
		EXPECT_EQ(executeCounter[3] + 1, command3_subcommand2->getExecuteCounter());
	}
}

TEST_F(CommandExecutorTest, unknownCommand) {
	Tokens tokens;

	tokens.push_back("unknown_command");
	EXPECT_THROW(execute(tokens), UnknownCommandException);

	tokens.push_back("P1");
	EXPECT_THROW(execute(tokens), UnknownCommandException);

	tokens.push_back("P2");
	EXPECT_THROW(execute(tokens), UnknownCommandException);

}

TEST_F(CommandExecutorTest, unknownSubCommand) {
	Tokens tokens;

	tokens.push_back("command3");

	tokens.push_back("P1");
	EXPECT_THROW(execute(tokens), UnknownSubcommandException);

	tokens.push_back("P2");
	EXPECT_THROW(execute(tokens), UnknownSubcommandException);
}

TEST_F(CommandExecutorTest, subCommandMissing) {
	Tokens tokens;

	tokens.push_back("command3");
	EXPECT_THROW(execute(tokens), SubcommandMissingException);
}

