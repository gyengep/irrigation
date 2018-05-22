#include "common.h"
#include "Command/CommandExecutor.h"
#include "CommandsTest.h"
#include "Logic/ContainerFactories.h"

#define GET_STARTTIMECONTAINER_MOCK(programId) dynamic_cast<StartTimeContainerMock&>(document->getPrograms().at(programId)->getStartTimes())


using namespace CommandLine;
using ::testing::_;


class StartTimeContainerMock : public StartTimeContainer {
public:

	MOCK_METHOD1(insert, IdType(const value_type& newItem));
	MOCK_METHOD1(erase, void(IdType id));
	MOCK_METHOD2(modify, void(IdType, const value_type& newItem));
};

class StartTimeContainerMockFactory : public Factory<StartTimeContainer> {
public:
	StartTimeContainer* create() { return new StartTimeContainerMock(); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(ListStartTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({})), TooFewArgumentsException);
}

TEST_F(ListStartTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooMuchArgumentsException);
}

TEST_F(ListStartTimeTest, invalidProgramID) {
	const std::string programID("1Q2");
	EXPECT_THROW(command->execute(Tokens({programID})), InvalidProgramIdException);
}

TEST_F(ListStartTimeTest, success) {
	const IdType programId = document->getPrograms().insert(new Program(""));
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void AddStartTimeTest::SetUp() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerMockFactory());
	CommandsTest<StartTimeAdd>::SetUp();
}

void AddStartTimeTest::TearDown() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerFactory());
	CommandsTest<StartTimeAdd>::TearDown();
}

TEST_F(AddStartTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooFewArgumentsException);
}

TEST_F(AddStartTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3", "P4"})), TooMuchArgumentsException);
}

TEST_F(AddStartTimeTest, invalidProgramID) {
	const std::string programID("jhvh");
	const std::string hour("1");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, hour, min})), InvalidProgramIdException);
}

TEST_F(AddStartTimeTest, invalidHour) {
	const std::string programID("0");
	const std::string hour("aa");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, hour, min})), InvalidParameterException);
}

TEST_F(AddStartTimeTest, invalidMin) {
	const std::string programID("0");
	const std::string hour("1");
	const std::string min("bb");
	EXPECT_THROW(command->execute(Tokens({programID, hour, min})), InvalidParameterException);
}

TEST_F(AddStartTimeTest, success) {
	const IdType programId = document->getPrograms().insert(new Program(""));
	const int hour = 10;
	const int min = 20;

	StartTimeContainerMock& startTimeContainerMock = GET_STARTTIMECONTAINER_MOCK(programId);

	EXPECT_CALL(startTimeContainerMock, insert(StartTime(hour, min))).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), std::to_string(hour), std::to_string(min)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void DelStartTimeTest::SetUp() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerMockFactory());
	CommandsTest<StartTimeDelete>::SetUp();
}

void DelStartTimeTest::TearDown() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerFactory());
	CommandsTest<StartTimeDelete>::TearDown();
}

TEST_F(DelStartTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooFewArgumentsException);
}

TEST_F(DelStartTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooMuchArgumentsException);
}

TEST_F(DelStartTimeTest, invalidProgramID) {
	const std::string programID("jhvh");
	const std::string startTimeID("1");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID})), InvalidProgramIdException);
}

TEST_F(DelStartTimeTest, invalidStartTimeID) {
	const std::string programID("0");
	const std::string startTimeID("abc");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID})), InvalidStartTimeIdException);
}

TEST_F(DelStartTimeTest, success) {
	const IdType startTimeId = 234U;
	const IdType programId = document->getPrograms().insert(new Program(""));

	StartTimeContainerMock& startTimeContainerMock = GET_STARTTIMECONTAINER_MOCK(programId);

	EXPECT_CALL(startTimeContainerMock, erase(startTimeId)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), std::to_string(startTimeId)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void SetStartTimeTest::SetUp() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerMockFactory());
	CommandsTest<StartTimeSet>::SetUp();
}

void SetStartTimeTest::TearDown() {
	StartTimeContainerFactoryHolder::setFactory(new StartTimeContainerFactory());
	CommandsTest<StartTimeSet>::TearDown();
}

TEST_F(SetStartTimeTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooFewArgumentsException);
}

TEST_F(SetStartTimeTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3", "P4", "P5"})), TooMuchArgumentsException);
}

TEST_F(SetStartTimeTest, invalidProgramID) {
	const std::string programID("jhvh");
	const std::string startTimeID("0");
	const std::string hour("1");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID, hour, min})), InvalidProgramIdException);
}

TEST_F(SetStartTimeTest, invalidStartTimeID) {
	const std::string programID("0");
	const std::string startTimeID("A");
	const std::string hour("1");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID, hour, min})), InvalidStartTimeIdException);
}

TEST_F(SetStartTimeTest, invalidHour) {
	const std::string programID("0");
	const std::string startTimeID("0");
	const std::string hour("aa");
	const std::string min("20");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID, hour, min})), InvalidParameterException);
}

TEST_F(SetStartTimeTest, invalidMin) {
	const std::string programID("0");
	const std::string startTimeID("0");
	const std::string hour("1");
	const std::string min("bb");
	EXPECT_THROW(command->execute(Tokens({programID, startTimeID, hour, min})), InvalidParameterException);
}

TEST_F(SetStartTimeTest, success) {
	const IdType startTimeId = 555;
	const int hour = 20;
	const int min = 30;
	const IdType programId = document->getPrograms().insert(new Program(""));

	StartTimeContainerMock& startTimeContainerMock = GET_STARTTIMECONTAINER_MOCK(programId);

	EXPECT_CALL(startTimeContainerMock, modify(startTimeId, StartTime(hour, min))).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), std::to_string(startTimeId), std::to_string(hour), std::to_string(min)})));
}

