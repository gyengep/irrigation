
#include "Common.h"
#include "Views/CommandLineView/CommandExecutor.h"


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

TEST(Command, parseProgramId) {
	EXPECT_EQ(0, Command::parseProgramId("0"));
	EXPECT_EQ(10, Command::parseProgramId("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseProgramId("4294967295"));
}

TEST(Command, parseProgramIdFailed) {
	EXPECT_THROW(Command::parseProgramId("-1"), InvalidProgramIdException);
	EXPECT_THROW(Command::parseProgramId("4294967296"), InvalidProgramIdException);
	EXPECT_THROW(Command::parseProgramId("4294 67296"), InvalidProgramIdException);
	EXPECT_THROW(Command::parseProgramId("1B2"), InvalidProgramIdException);
	EXPECT_THROW(Command::parseProgramId("abc"), InvalidProgramIdException);
}

TEST(Command, parseStartTimeId) {
	EXPECT_EQ(0, Command::parseStartTimeId("0"));
	EXPECT_EQ(10, Command::parseStartTimeId("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseStartTimeId("4294967295"));
}

TEST(Command, parseStartTimeIdFailed) {
	EXPECT_THROW(Command::parseStartTimeId("-1"), InvalidStartTimeIdException);
	EXPECT_THROW(Command::parseStartTimeId("4294967296"), InvalidStartTimeIdException);
	EXPECT_THROW(Command::parseStartTimeId("4294 67296"), InvalidStartTimeIdException);
	EXPECT_THROW(Command::parseStartTimeId("1B2"), InvalidStartTimeIdException);
	EXPECT_THROW(Command::parseStartTimeId("abc"), InvalidStartTimeIdException);
}

TEST(Command, parseRunTimeId) {
	EXPECT_EQ(0, Command::parseRunTimeId("0"));
	EXPECT_EQ(10, Command::parseRunTimeId("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseRunTimeId("4294967295"));
}

TEST(Command, parseRunTimeIdFailed) {
	EXPECT_THROW(Command::parseRunTimeId("-1"), InvalidRunTimeIdException);
	EXPECT_THROW(Command::parseRunTimeId("4294967296"), InvalidRunTimeIdException);
	EXPECT_THROW(Command::parseRunTimeId("4294 67296"), InvalidRunTimeIdException);
	EXPECT_THROW(Command::parseRunTimeId("1B2"), InvalidRunTimeIdException);
	EXPECT_THROW(Command::parseRunTimeId("abc"), InvalidRunTimeIdException);
}

TEST(Command, parseUnsigned) {
	EXPECT_EQ(0, Command::parseUnsigned("0"));
	EXPECT_EQ(10, Command::parseUnsigned("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseUnsigned("4294967295"));
}

TEST(Command, parseUnsignedFailed) {
	EXPECT_THROW(Command::parseUnsigned("-1"), InvalidParameterException);
	EXPECT_THROW(Command::parseUnsigned("4294967296"), InvalidParameterException);
	EXPECT_THROW(Command::parseUnsigned("4294 67296"), InvalidParameterException);
	EXPECT_THROW(Command::parseUnsigned("1B2"), InvalidParameterException);
	EXPECT_THROW(Command::parseUnsigned("abc"), InvalidParameterException);
}

TEST(Command, parseZoneId) {
	EXPECT_EQ(0, Command::parseZoneId("0"));
	EXPECT_EQ(10, Command::parseZoneId("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseZoneId("4294967295"));
}

TEST(Command, parseZoneIdFailed) {
	EXPECT_THROW(Command::parseZoneId("-1"), InvalidZoneIdException);
	EXPECT_THROW(Command::parseZoneId("4294967296"), InvalidZoneIdException);
	EXPECT_THROW(Command::parseZoneId("4294 67296"), InvalidZoneIdException);
	EXPECT_THROW(Command::parseZoneId("1B2"), InvalidZoneIdException);
	EXPECT_THROW(Command::parseZoneId("abc"), InvalidZoneIdException);
}

TEST(Command, parseValveId) {
	EXPECT_EQ(0, Command::parseValveId("0"));
	EXPECT_EQ(10, Command::parseValveId("10"));
	EXPECT_EQ(0xFFFFFFFF, Command::parseValveId("4294967295"));
}

TEST(Command, parseValveIdFailed) {
	EXPECT_THROW(Command::parseValveId("-1"), InvalidValveIdException);
	EXPECT_THROW(Command::parseValveId("4294967296"), InvalidValveIdException);
	EXPECT_THROW(Command::parseValveId("4294 67296"), InvalidValveIdException);
	EXPECT_THROW(Command::parseValveId("1B2"), InvalidValveIdException);
	EXPECT_THROW(Command::parseValveId("abc"), InvalidValveIdException);
}

TEST(Command, parseOnOff) {
	EXPECT_EQ(true, Command::parseOnOff("on"));
	EXPECT_EQ(false, Command::parseOnOff("off"));
}

TEST(Command, parseOnOffFailed) {
	EXPECT_THROW(Command::parseOnOff("on "), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff(" on"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("on1"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("-1"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("4294967296"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("4294 67296"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("1B2"), InvalidParameterException);
	EXPECT_THROW(Command::parseOnOff("abc"), InvalidParameterException);
}
