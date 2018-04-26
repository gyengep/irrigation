
#include "Common.h"
#include "Command/CommandExecutor.h"

using ::testing::_;


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
	Tokens tokens({"command1"});

	EXPECT_CALL(*command1, execute(Tokens())).Times(1);
	EXPECT_CALL(*command2, execute(_)).Times(0);
	EXPECT_NO_THROW(commandExecutor.execute(tokens));
}

TEST_F(CommandExecutorTest, executeSuccess2) {
	Tokens tokens({"command2"});

	EXPECT_CALL(*command1, execute(_)).Times(0);
	EXPECT_CALL(*command2, execute(Tokens())).Times(1);
	EXPECT_NO_THROW(commandExecutor.execute(tokens));
}

TEST_F(CommandExecutorTest, parameters) {
	const std::string p1("P1");
	Tokens tokens({"command1", p1});

	EXPECT_CALL(*command1, execute(Tokens({p1}))).Times(1);
	EXPECT_CALL(*command2, execute(_)).Times(0);
	EXPECT_NO_THROW(commandExecutor.execute(tokens));
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
