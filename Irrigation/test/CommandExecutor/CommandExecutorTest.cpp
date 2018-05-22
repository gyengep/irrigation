#include "CommandExecutorTest.h"
#include "CommandExecutor/Exceptions.h"

using namespace std;
using ::testing::_;


void CommandExecutorTest::SetUp() {
	command1 = new CommandTest("command1");
	command2 = new CommandTest("command2");

	commandExecutor.addCommand(command1);
	commandExecutor.addCommand(command2);
}

void CommandExecutorTest::TearDown() {
}

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

TEST_F(CommandExecutorTest, invalidCommand) {
	EXPECT_THROW(commandExecutor.addCommand(nullptr), invalid_argument);
	EXPECT_THROW(commandExecutor.addCommand(new CommandTest(NULL)), invalid_argument);
	EXPECT_THROW(commandExecutor.addCommand(new CommandTest("")), invalid_argument);
}

TEST_F(CommandExecutorTest, parameters) {
	const string p1("P1");
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

TEST_F(CommandExecutorTest, invalidToken) {
	EXPECT_THROW(commandExecutor.execute(Tokens()), invalid_argument);
	EXPECT_THROW(commandExecutor.execute(Tokens({""})), invalid_argument);
}
