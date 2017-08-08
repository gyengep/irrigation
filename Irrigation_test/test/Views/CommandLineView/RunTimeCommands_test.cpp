#include "common.h"
#include "Commands_test.h"

#include "Command/CommandExecutor.h"
#include "Logic/ContainerFactories.h"

#define GET_RUNTIMECONTAINER_MOCK(programId) dynamic_cast<RunTimeContainerMock&>(document->getPrograms().at(programId)->getRunTimes())


using namespace CommandLine;
using ::testing::_;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class RunTimeContainerMock : public RunTimeContainer {
public:
	RunTimeContainerMock() {}

	MOCK_METHOD2(modify, void(IdType, const value_type& newItem));
};

class RunTimeContainerMockFactory : public Factory<RunTimeContainer> {
public:
	RunTimeContainer* create() { return new RunTimeContainerMock(); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(ListRunTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({})), TooFewArgumentsException);
}

TEST_F(ListRunTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooMuchArgumentsException);
}

TEST_F(ListRunTimeTest, invalidProgramID) {
	const std::string programID("1Q2");
	EXPECT_THROW(command->execute(Tokens({programID})), InvalidProgramIdException);
}

TEST_F(ListRunTimeTest, success) {
	const IdType programId = document->getPrograms().insert(new Program(""));

	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SetRunTimeTest::SetUp() {
	RunTimeContainerFactoryHolder::setFactory(new RunTimeContainerMockFactory());
	CommandsTest<RunTimeSet>::SetUp();
}

void SetRunTimeTest::TearDown() {
	RunTimeContainerFactoryHolder::setFactory(new RunTimeContainerFactory());
	CommandsTest<RunTimeSet>::SetUp();
}

TEST_F(SetRunTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooFewArgumentsException);
}

TEST_F(SetRunTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3", "P4"})), TooMuchArgumentsException);
}

TEST_F(SetRunTimeTest, invalidProgramID) {
	const std::string programID("jhvh");
	const std::string runTimeID("0");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, runTimeID, min})), InvalidProgramIdException);
}

TEST_F(SetRunTimeTest, invalidRunTimeID) {
	const std::string programID("0");
	const std::string runTimeID("A");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, runTimeID, min})), InvalidRunTimeIdException);
}

TEST_F(SetRunTimeTest, invalidMin) {
	const std::string programID("0");
	const std::string runTimeID("0");
	const std::string min("bb");
	EXPECT_THROW(command->execute(Tokens({programID, runTimeID, min})), InvalidParameterException);
}

TEST_F(SetRunTimeTest, success) {
	const IdType runTimeId = 1;
	const int min = 23;
	const IdType programId = document->getPrograms().insert(new Program(""));

	RunTimeContainerMock& runTimeContainerMock = GET_RUNTIMECONTAINER_MOCK(programId);

	EXPECT_CALL(runTimeContainerMock, modify(runTimeId, min)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), std::to_string(runTimeId), std::to_string(min)})));
}
