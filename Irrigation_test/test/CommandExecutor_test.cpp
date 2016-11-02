#include "common.h"
#include "CommandExecutor.h"


class CommandTest : public Command {
	Tokens tokens;
	int executeCounter;

public:

	CommandTest(const char* command, const char* subCommand) :
		Command(NULL, NULL, command, subCommand),
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

TEST_F(CommandExecutorTest, executeSuccess1) {
	Tokens tokens;
	tokens.push_back("command1");

	execute(tokens);

	EXPECT_EQ(1, command1->getExecuteCounter());
	EXPECT_EQ(0, command2->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand1->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, executeSuccess2) {
	Tokens tokens;
	tokens.push_back("command2");

	execute(tokens);

	EXPECT_EQ(0, command1->getExecuteCounter());
	EXPECT_EQ(1, command2->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand1->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, executeSuccess3) {
	Tokens tokens;
	tokens.push_back("command3");
	tokens.push_back("subcommand1");

	execute(tokens);

	EXPECT_EQ(0, command1->getExecuteCounter());
	EXPECT_EQ(0, command2->getExecuteCounter());
	EXPECT_EQ(1, command3_subcommand1->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, executeSuccess4) {
	Tokens tokens;
	tokens.push_back("command3");
	tokens.push_back("subcommand2");

	execute(tokens);

	EXPECT_EQ(0, command1->getExecuteCounter());
	EXPECT_EQ(0, command2->getExecuteCounter());
	EXPECT_EQ(0, command3_subcommand1->getExecuteCounter());
	EXPECT_EQ(1, command3_subcommand2->getExecuteCounter());
}

TEST_F(CommandExecutorTest, parameters) {
	Tokens tokens;
	tokens.push_back("command1");

	execute(tokens);
	EXPECT_EQ(0, command1->getTokens().size());

	tokens.push_back("P1");
	execute(tokens);
	EXPECT_EQ(1, command1->getTokens().size());
	EXPECT_STREQ("P1", command1->getTokens().front().c_str());
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

TEST(Command, parseUID) {
	EXPECT_EQ(0, Command::parseId("0", "0"));
	EXPECT_EQ(10, Command::parseId("10", "10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseId("4294967295", "4294967295"));
}

TEST(Command, parseUIDFailed) {
	EXPECT_THROW(Command::parseId("-1", ""), CommandLineException);
	EXPECT_THROW(Command::parseId("4294967296", ""), CommandLineException);
	EXPECT_THROW(Command::parseId("4294 67296", ""), CommandLineException);
	EXPECT_THROW(Command::parseId("1B2", ""), CommandLineException);
	EXPECT_THROW(Command::parseId("abc", ""), CommandLineException);
}

TEST(Command, parseUINT) {
	EXPECT_EQ(0, Command::parseUInt("0", "0"));
	EXPECT_EQ(10, Command::parseUInt("10", "10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseUInt("4294967295", "4294967295"));
}

TEST(Command, parseUINTFailed) {
	EXPECT_THROW(Command::parseUInt("-1", ""), CommandLineException);
	EXPECT_THROW(Command::parseUInt("4294967296", ""), CommandLineException);
	EXPECT_THROW(Command::parseUInt("4294 67296", ""), CommandLineException);
	EXPECT_THROW(Command::parseUInt("1B2", ""), CommandLineException);
	EXPECT_THROW(Command::parseUInt("abc", ""), CommandLineException);
}

TEST(Command, parseOnOff) {
	EXPECT_EQ(true, Command::parseOnOff("on", "on"));
	EXPECT_EQ(false, Command::parseOnOff("off", "off"));
}

TEST(Command, parseOnOffFailed) {
	EXPECT_THROW(Command::parseOnOff("on ", "on_"), CommandLineException);
	EXPECT_THROW(Command::parseOnOff(" on", "_on"), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("on1", "on1"), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("-1", ""), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("4294967296", ""), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("4294 67296", ""), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("1B2", ""), CommandLineException);
	EXPECT_THROW(Command::parseOnOff("abc", ""), CommandLineException);
}
