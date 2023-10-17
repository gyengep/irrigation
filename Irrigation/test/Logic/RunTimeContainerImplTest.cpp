#include "Logic/Impl/RunTimeContainerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Mocks/MockRunTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplTest, defaultConstructor) {
	auto mockRunTimeFactory = std::make_shared<StrictMock<MockRunTimeFactory>>(6);
	EXPECT_CALL(*mockRunTimeFactory, create()).Times(6);

	const RunTimeContainerImpl runTimes(mockRunTimeFactory);

	ASSERT_THAT(runTimes, SizeIs(mockRunTimeFactory->mockRunTimes.size()));

	for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(next(runTimes.begin(), i)->second, Eq(*next(mockRunTimeFactory->mockRunTimes.begin(), i)));
	}
}

TEST(RunTimeContainerImplTest, initializerConstructor) {
	const std::initializer_list<RunTimePtr> initializer {
		std::make_shared<MockRunTime>(),
		std::make_shared<MockRunTime>(),
		std::make_shared<MockRunTime>(),
		std::make_shared<MockRunTime>(),
		std::make_shared<MockRunTime>(),
		std::make_shared<MockRunTime>()
	};

	const RunTimeContainerImpl runTimes(initializer);

	ASSERT_THAT(runTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(next(runTimes.begin(), i)->second, Eq(*next(initializer.begin(), i)));
	}
}

TEST(RunTimeContainerImplTest, initializerConstructorWithWrongInitializer) {
	EXPECT_THROW(
		RunTimeContainerImpl({
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>()
		}),
		std::logic_error
	);

	EXPECT_THROW(
		RunTimeContainerImpl({
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>(),
			std::make_shared<MockRunTime>()
		}),
		std::logic_error
	);
}

TEST(RunTimeContainerImplTest, size) {
	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));
	EXPECT_THAT(runTimes, SizeIs(ZoneHandler::getZoneCount()));
}

TEST(RunTimeContainerImplTest, id) {
	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
	}
}

TEST(RunTimeContainerImplTest, at) {
	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(runTimes.at(i), Eq(next(runTimes.begin(), i)->second));
	}
}

TEST(RunTimeContainerImplTest, atInvalid) {
	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

TEST(RunTimeContainerImplTest, toDurationList) {
	auto mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);

	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[0], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(0)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[1], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(50)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[2], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(12)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[3], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(130)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[4], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(23)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[5], toDuration()).Times(1).WillOnce(Return(std::chrono::seconds(60)));

	EXPECT_THAT(
			RunTimeContainerImpl(mockRunTimeFactory).toDurationList(),
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

TEST(RunTimeContainerImplTest, toDurationsStatic) {
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
