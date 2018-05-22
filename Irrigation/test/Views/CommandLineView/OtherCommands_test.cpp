#include "Command/CommandExecutor.h"
#include "Logic/ContainerFactories.h"
#include "Model/Application.h"
#include "Views/CommandLineView/CommandLineView.h"
#include "CommandsTest.h"

#define GET_APPLICATION_MOCK() dynamic_cast<ApplicationMock&>(Application::getInstance())


using namespace CommandLine;
using ::testing::_;


class ApplicationMock : public Application {
public:

	MOCK_METHOD0(terminate, void());
};

class ApplicationMockFactory : public Factory<Application> {
public:
	Application* create() { return new ApplicationMock(); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ExitTest::SetUp() {
	Application::setFactory(new ApplicationMockFactory());
	CommandsTest<Exit>::SetUp();
}

void ExitTest::TearDown() {
	Application::setFactory(new ApplicationFactory());
	CommandsTest<Exit>::TearDown();
}

TEST_F(ExitTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(ExitTest, success) {
	ApplicationMock& applicationMock = GET_APPLICATION_MOCK();

	EXPECT_CALL(applicationMock, terminate()).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void HelpTest::SetUp() {
	view.reset(new CommandLineView());
	command.reset(new Help(view.get(), NULL));
}

void HelpTest::TearDown() {
}

TEST_F(HelpTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(HelpTest, success) {
	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


