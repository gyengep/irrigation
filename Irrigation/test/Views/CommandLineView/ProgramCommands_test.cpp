#include "common.h"
#include "Command/CommandExecutor.h"
#include "CommandsTest.h"
#include "Logic/ContainerFactories.h"

#define GET_PROGRAMCONTAINER_MOCK() dynamic_cast<ProgramContainerMock&>(document->getPrograms())


using namespace CommandLine;
using ::testing::_;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ProgramMock : public Program {
public:
	ProgramMock() : Program("") {}

	MOCK_METHOD1(setName, void(const std::string& name));
};

class ProgramContainerMock : public ProgramContainer {
public:

	MOCK_METHOD1(insert, IdType(const value_type& newItem));
	MOCK_METHOD1(erase, void(IdType id));
	MOCK_METHOD2(move, void(IdType id, size_t newPosition));
};

class ProgramContainerMockFactory : public Factory<ProgramContainer> {
public:
	ProgramContainer* create() { return new ProgramContainerMock(); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(ListProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(ListProgramTest, success) {
	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(ShowProgramTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens()), TooFewArgumentsException);
}

TEST_F(ShowProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooMuchArgumentsException);
}

TEST_F(ShowProgramTest, invalidProgramID) {
	const std::string programID("1Q2");
	EXPECT_THROW(command->execute(Tokens({programID})), InvalidProgramIdException);
}

TEST_F(ShowProgramTest, success) {
	const IdType programId = document->getPrograms().insert(new Program(""));
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void AddProgramTest::SetUp() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerMockFactory());
	CommandsTest<ProgramAdd>::SetUp();
}

void AddProgramTest::TearDown() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerFactory());
	CommandsTest<ProgramAdd>::TearDown();
}

TEST_F(AddProgramTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens()), TooFewArgumentsException);
}

TEST_F(AddProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooMuchArgumentsException);
}

TEST_F(AddProgramTest, success) {
	ProgramContainerMock& programContainerMock = GET_PROGRAMCONTAINER_MOCK();

	const std::string name("poiuzt");
	EXPECT_CALL(programContainerMock, insert(_)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({name})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void DelProgramTest::SetUp() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerMockFactory());
	CommandsTest<ProgramDelete>::SetUp();
}

void DelProgramTest::TearDown() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerFactory());
	CommandsTest<ProgramDelete>::TearDown();
}

TEST_F(DelProgramTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens()), TooFewArgumentsException);
}

TEST_F(DelProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2"})), TooMuchArgumentsException);
}

TEST_F(DelProgramTest, invalidProgramID) {
	const std::string programID("12A");
	EXPECT_THROW(command->execute(Tokens({programID})), InvalidProgramIdException);
}

TEST_F(DelProgramTest, success) {
	ProgramContainerMock& programContainerMock = GET_PROGRAMCONTAINER_MOCK();

	const IdType programId = 123U;
	EXPECT_CALL(programContainerMock, erase(programId)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId)})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

TEST_F(RenProgramTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooFewArgumentsException);
}

TEST_F(RenProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooMuchArgumentsException);
}

TEST_F(RenProgramTest, invalidProgramID) {
	const std::string programID("abc");
	const std::string name("name");
	EXPECT_THROW(command->execute(Tokens({programID, name})), InvalidProgramIdException);
}

TEST_F(RenProgramTest, success) {
	ProgramMock* programMock = new ProgramMock();

	const std::string name("abc");
	const IdType programId = document->getPrograms().insert(programMock);
	EXPECT_CALL(*programMock, setName(name)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), name})));
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void MoveProgramTest::SetUp() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerMockFactory());
	CommandsTest<ProgramMove>::SetUp();
}

void MoveProgramTest::TearDown() {
	ProgramContainerFactoryHolder::setFactory(new ProgramContainerFactory());
	CommandsTest<ProgramMove>::TearDown();
}

TEST_F(MoveProgramTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooFewArgumentsException);
}

TEST_F(MoveProgramTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooMuchArgumentsException);
}

TEST_F(MoveProgramTest, invalidProgramID) {
	const std::string programID("abc");
	const std::string position("0");
	EXPECT_THROW(command->execute(Tokens({programID, position})), InvalidProgramIdException);
}

TEST_F(MoveProgramTest, invalidPosition) {
	const std::string programID("0");
	const std::string position("abc");
	EXPECT_THROW(command->execute(Tokens({programID, position})), InvalidParameterException);
}

TEST_F(MoveProgramTest, success) {
	ProgramContainerMock& programContainerMock = GET_PROGRAMCONTAINER_MOCK();

	const IdType programId = 123U;
	const size_t position = 0U;
	EXPECT_CALL(programContainerMock, move(programId, position)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(programId), std::to_string(position)})));
}

