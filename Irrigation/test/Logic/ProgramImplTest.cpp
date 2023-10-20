#include "ProgramImplTest.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramImplConstructorTest, defaultConstructor) {
	const auto mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	const auto mockRunTimeContainer = std::make_shared<MockRunTimeContainer>();
	const auto mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();

	const ProgramImpl program(
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
	);

	EXPECT_TRUE(program.isEnabled());
	EXPECT_THAT(program.getName(), Eq(""));
	EXPECT_THAT(program.getAdjustment(), Eq(100));
	EXPECT_THAT(program.getSchedulerType(), Eq(SchedulerType::WEEKLY));
	EXPECT_THAT(&program.getSchedulerContainer(), Eq(mockSchedulerContainer.get()));
	EXPECT_THAT(&program.getRunTimeContainer(), Eq(mockRunTimeContainer.get()));
	EXPECT_THAT(&program.getStartTimeContainer(), Eq(mockStartTimeContainer.get()));
}

TEST(ProgramImplConstructorTest, parametrizedConstructor) {
	const bool enabled(false);
	const std::string name("gcuuzg");
	const unsigned adjustment = 78;
	const SchedulerType schedulerType = SchedulerType::EVERY_DAY;

	const auto mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	const auto mockRunTimeContainer = std::make_shared<MockRunTimeContainer>();
	const auto mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();

	const ProgramImpl program(
		enabled, name, adjustment, schedulerType,
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
	);

	EXPECT_THAT(program.isEnabled(), Eq(enabled));
	EXPECT_THAT(program.getName(), Eq(name));
	EXPECT_THAT(program.getAdjustment(), Eq(adjustment));
	EXPECT_THAT(program.getSchedulerType(), Eq(schedulerType));
	EXPECT_THAT(&program.getSchedulerContainer(), Eq(mockSchedulerContainer.get()));
	EXPECT_THAT(&program.getRunTimeContainer(), Eq(mockRunTimeContainer.get()));
	EXPECT_THAT(&program.getStartTimeContainer(), Eq(mockStartTimeContainer.get()));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramImplFactoryTest, create) {
	auto mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
	auto mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();
	auto mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();

	auto mockSchedulerContainerFactory = std::make_shared<StrictMock<MockSchedulerContainerFactory>>();
	auto mockRunTimeContainerFactory = std::make_shared<StrictMock<MockRunTimeContainerFactory>>();
	auto mockStartTimeContainerFactory = std::make_shared<StrictMock<MockStartTimeContainerFactory>>();

	EXPECT_CALL(*mockSchedulerContainerFactory, create()).
			Times(1).
			WillOnce(Return(mockSchedulerContainer));

	EXPECT_CALL(*mockRunTimeContainerFactory, create()).
			Times(1).
			WillOnce(Return(mockRunTimeContainer));

	EXPECT_CALL(*mockStartTimeContainerFactory, create()).
			Times(1).
			WillOnce(Return(mockStartTimeContainer));

	ProgramPtr program = ProgramImplFactory(
			mockSchedulerContainerFactory,
			mockRunTimeContainerFactory,
			mockStartTimeContainerFactory
		).create();

	ASSERT_THAT(program, Not(IsNull()));

	EXPECT_THAT(&program->getSchedulerContainer(), Eq(mockSchedulerContainer.get()));
	EXPECT_THAT(&program->getRunTimeContainer(), Eq(mockRunTimeContainer.get()));
	EXPECT_THAT(&program->getStartTimeContainer(), Eq(mockStartTimeContainer.get()));
}

///////////////////////////////////////////////////////////////////////////////

void ProgramImplTest::SetUp() {
	mockSchedulerContainer = std::make_shared<StrictMock<MockSchedulerContainer>>();
	mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();

	program = std::make_shared<ProgramImpl>(
		mockSchedulerContainer,
		mockRunTimeContainer,
		mockStartTimeContainer
	);
}

void ProgramImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramImplTest, setEnabled) {
	const bool expected1 = true;
	const bool expected2 = false;

	program->setEnabled(expected1);
	EXPECT_THAT(program->isEnabled(), Eq(expected1));

	program->setEnabled(expected2);
	EXPECT_THAT(program->isEnabled(), Eq(expected2));
}

TEST_F(ProgramImplTest, setName) {
	const std::string expected1("AbcXyz");
	const std::string expected2("9876543210");

	program->setName(expected1);
	EXPECT_THAT(program->getName(), Eq(expected1));

	program->setName(expected2);
	EXPECT_THAT(program->getName(), Eq(expected2));
}

TEST_F(ProgramImplTest, setAdjustment) {
	const unsigned expected1 = 70;
	const unsigned expected2 = 71;

	program->setAdjustment(expected1);
	EXPECT_THAT(program->getAdjustment(), Eq(expected1));

	program->setAdjustment(expected2);
	EXPECT_THAT(program->getAdjustment(), Eq(expected2));
}

TEST_F(ProgramImplTest, setSchedulerType) {
	const SchedulerType expected1 = SchedulerType::EVERY_DAY;
	const SchedulerType expected2 = SchedulerType::WEEKLY;

	program->setSchedulerType(expected1);
	EXPECT_THAT(program->getSchedulerType(), Eq(expected1));

	program->setSchedulerType(expected2);
	EXPECT_THAT(program->getSchedulerType(), Eq(expected2));
}
