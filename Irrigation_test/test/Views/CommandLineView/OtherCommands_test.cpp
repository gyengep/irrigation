#include "common.h"
#include "Commands_test.h"

#include "Command/CommandExecutor.h"
#include "Logic/ContainerFactories.h"

using namespace CommandLine;
using ::testing::_;


TEST_F(ExitTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(ExitTest, success) {
	ValvesMock& valvesMock = GET_VALVES_MOCK();

	for (size_t index = 0; index < VALVE_COUNT; index++) {
		EXPECT_CALL(valvesMock, activate(index, false)).Times(1);
	}

	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(HelpTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(HelpTest, success) {
	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


