#include "Logic/RunTimeContainerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Mocks/MockRunTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplTest, defaultConstructor) {
	const std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);
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

///////////////////////////////////////////////////////////////////////////////

#define RUN_TIME_DTO(i)  	RunTimeDTO().setMinutes(10 + i).setSeconds(20 + i)


TEST(RunTimeContainerImplTest, updateFromDtoList) {
	const std::list<RunTimeDTO> runTimeDtoList {
		RUN_TIME_DTO(4).setId(0),
		RUN_TIME_DTO(5).setId(1),
		RUN_TIME_DTO(6).setId(2),
		RUN_TIME_DTO(7).setId(3),
		RUN_TIME_DTO(8).setId(4),
		RUN_TIME_DTO(9).setId(5)
	};

	std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);

	for (size_t i = 0; i < runTimeDtoList.size(); ++i) {
		EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], updateFromRunTimeDto(*next(runTimeDtoList.begin(), i)))
			.Times(1);
	}

	RunTimeContainerImpl runTimes(mockRunTimeFactory);
	EXPECT_THAT(runTimes, SizeIs(6));

	runTimes.updateFromRunTimeDtoList(runTimeDtoList);
	ASSERT_THAT(runTimes, SizeIs(runTimeDtoList.size()));

	for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(next(runTimes.begin(), i)->second, Eq(*next(mockRunTimeFactory->mockRunTimes.begin(), i)));
	}
}

TEST(RunTimeContainerImplTest, updateFromDtoList_zeroItem) {
	const std::list<RunTimeDTO> runTimeDtoList({
	});

	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerImplTest, updateFromDtoList_oneItem) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO()
	});

	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerImplTest, updateFromDtoList_lessThanExpectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerImplTest, updateFromDtoList_expectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	const std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);
	RunTimeContainerImpl runTimes(mockRunTimeFactory);

	for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
		EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], updateFromRunTimeDto(_))
			.Times(1);
	}

	EXPECT_NO_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList));
}

TEST(RunTimeContainerImplTest, updateFromDtoList_moreThanExpectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	RunTimeContainerImpl runTimes(std::make_shared<MockRunTimeFactory>(6));

	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerImplTest, updateFromDtoList_expectedCountOfItemsWithWrongId) {
	{
		const std::list<RunTimeDTO> runTimeDtoList({
			RunTimeDTO().setId(5),
			RunTimeDTO().setId(4),
			RunTimeDTO().setId(3),
			RunTimeDTO().setId(2),
			RunTimeDTO().setId(1),
			RunTimeDTO().setId(0)
		});

		const std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);
		RunTimeContainerImpl runTimes(mockRunTimeFactory);

		for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
			EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], updateFromRunTimeDto(_))
				.Times(Between(0, 1));
		}

		EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
	}

	{
		const std::list<RunTimeDTO> runTimeDtoList({
			RunTimeDTO(),
			RunTimeDTO(),
			RunTimeDTO().setId(0),
			RunTimeDTO(),
			RunTimeDTO(),
			RunTimeDTO()
		});

		const std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);
		RunTimeContainerImpl runTimes(mockRunTimeFactory);

		for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); ++i) {
			EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], updateFromRunTimeDto(_))
				.Times(Between(0, 1));
		}

		EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplTest, toRunTimeDtoList) {
	const std::list<RunTimeDTO> expected {
		RUN_TIME_DTO(0).setId(0),
		RUN_TIME_DTO(1).setId(1),
		RUN_TIME_DTO(2).setId(2),
		RUN_TIME_DTO(3).setId(3),
		RUN_TIME_DTO(4).setId(4),
		RUN_TIME_DTO(5).setId(5)
	};

	const std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>(6);
	RunTimeContainerImpl runTimes(mockRunTimeFactory);

	for (size_t i = 0; i < mockRunTimeFactory->mockRunTimes.size(); i++) {
		EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], toRunTimeDto())
			.Times(1)
			.WillOnce(Return(RUN_TIME_DTO(i)));
	}

	EXPECT_THAT(runTimes.toRunTimeDtoList(), ContainerEq(expected));
}
