#include "RunTimeContainerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplConstructorTest, defaultConstructor) {
	std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<StrictMock<MockRunTimeFactory>>();

	EXPECT_CALL(*mockRunTimeFactory, create()).
			Times(6).
			WillRepeatedly(Invoke(mockRunTimeFactory.get(), &MockRunTimeFactory::createMockRunTime));

	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());

	const RunTimeContainerImpl runTimeContainer(mockRunTimeFactory);

	ASSERT_THAT(runTimeContainer, SizeIs(mockRunTimeFactory->mockRunTimes.size()));

	for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
		EXPECT_THAT(std::next(runTimeContainer.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(std::next(runTimeContainer.begin(), i)->second, Eq(*std::next(mockRunTimeFactory->mockRunTimes.begin(), i)));
	}
}

TEST(RunTimeContainerImplConstructorTest, initializerConstructor) {
	const std::initializer_list<RunTimePtr> initializer {
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>()
	};

	const RunTimeContainerImpl runTimeContainer(initializer);

	ASSERT_THAT(runTimeContainer, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(std::next(runTimeContainer.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(std::next(runTimeContainer.begin(), i)->second, Eq(*std::next(initializer.begin(), i)));
	}
}

TEST_F(RunTimeContainerImplTest, initializerConstructorWithWrongInitializer) {
	EXPECT_THROW(
		RunTimeContainerImpl({
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>()
		}),
		std::logic_error
	);

	EXPECT_THROW(
		RunTimeContainerImpl({
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>(),
			std::make_shared<StrictMock<MockRunTime>>()
		}),
		std::logic_error
	);
}

///////////////////////////////////////////////////////////////////////////////

void RunTimeContainerImplTest::SetUp() {
	mockRunTimeFactory = std::make_shared<StrictMock<MockRunTimeFactory>>();

	EXPECT_CALL(*mockRunTimeFactory, create()).
			Times(6).
			WillRepeatedly(Invoke(mockRunTimeFactory.get(), &MockRunTimeFactory::createMockRunTime));

	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());
	mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());

	runTimeContainer = std::make_shared<RunTimeContainerImpl>(
			mockRunTimeFactory
		);
}

void RunTimeContainerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeContainerImplTest, size) {
	EXPECT_THAT(*runTimeContainer, SizeIs(ZoneHandler::getZoneCount()));
}

TEST_F(RunTimeContainerImplTest, id) {
	for (size_t i = 0; i < runTimeContainer->size(); ++i) {
		EXPECT_THAT(std::next(runTimeContainer->begin(), i)->first, Eq(IdType(i)));
	}
}

TEST_F(RunTimeContainerImplTest, at) {
	for (size_t i = 0; i < runTimeContainer->size(); ++i) {
		EXPECT_THAT(runTimeContainer->at(i), Eq(std::next(runTimeContainer->begin(), i)->second));
		EXPECT_THAT(Const(runTimeContainer)->at(i), Eq(std::next(runTimeContainer->begin(), i)->second));
	}
}

TEST_F(RunTimeContainerImplTest, at_notExisting) {
	EXPECT_THROW(runTimeContainer->at(ZoneHandler::getZoneCount()), NoSuchElementException);
	EXPECT_THROW(Const(runTimeContainer)->at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

TEST_F(RunTimeContainerImplTest, toDurationList) {

	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[0], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(0)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[1], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(50)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[2], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(12)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[3], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(130)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[4], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(23)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[5], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(60)));

	EXPECT_THAT(
			runTimeContainer->toDurationList(),
			ContainerEq(
				DurationList {
					std::chrono::seconds(0),
					std::chrono::seconds(50),
					std::chrono::seconds(12),
					std::chrono::seconds(130),
					std::chrono::seconds(23),
					std::chrono::seconds(60)
				}
			)
		);
}

TEST_F(RunTimeContainerImplTest, toDurationsStatic) {
	EXPECT_THAT(
			RunTimeContainer::toDurationList(
				std::list<RunTimeDTO>{
					RunTimeDTO(0, 0),
					RunTimeDTO(0, 12),
					RunTimeDTO(1, 25),
					RunTimeDTO(0, 33),
					RunTimeDTO(5, 27),
					RunTimeDTO(3, 10)
				}
			),
			ContainerEq(
				DurationList {
					std::chrono::minutes(0) + std::chrono::seconds(0),
					std::chrono::minutes(0) + std::chrono::seconds(12),
					std::chrono::minutes(1) + std::chrono::seconds(25),
					std::chrono::minutes(0) + std::chrono::seconds(33),
					std::chrono::minutes(5) + std::chrono::seconds(27),
					std::chrono::minutes(3) + std::chrono::seconds(10),
				}
			)
		);
}
