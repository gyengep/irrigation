#include <gmock/gmock.h>
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/RunTimeImpl.h"
#include "Mocks/MockRunTime.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, defaultConstructor) {
	const RunTimeImpl expected(0);
	const RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());

	ASSERT_THAT(runTimes, SizeIs(6));

	EXPECT_THAT(runTimes.at(0), Pointee(Eq(std::ref(expected))));
	EXPECT_THAT(runTimes.at(1), Pointee(Eq(std::ref(expected))));
	EXPECT_THAT(runTimes.at(2), Pointee(Eq(std::ref(expected))));
	EXPECT_THAT(runTimes.at(3), Pointee(Eq(std::ref(expected))));
	EXPECT_THAT(runTimes.at(4), Pointee(Eq(std::ref(expected))));
	EXPECT_THAT(runTimes.at(5), Pointee(Eq(std::ref(expected))));
}

TEST(RunTimeContainerTest, initializerConstructor) {
	const std::initializer_list<RunTimePtr> initializer {
		std::make_shared<RunTimeImpl>(10),
		std::make_shared<RunTimeImpl>(11),
		std::make_shared<RunTimeImpl>(12),
		std::make_shared<RunTimeImpl>(13),
		std::make_shared<RunTimeImpl>(14),
		std::make_shared<RunTimeImpl>(15)
	};

	const RunTimeContainer runTimes(initializer);

	ASSERT_THAT(runTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
		EXPECT_THAT(next(runTimes.begin(), i)->second.get(), next(initializer.begin(), i)->get());
		EXPECT_THAT(next(runTimes.begin(), i)->second->getSeconds(), 10 + i);
	}
}

TEST(RunTimeContainerTest, initializerConstructorWithWrongInitializer) {
	EXPECT_THROW(
		RunTimeContainer({
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>()
		}),
		std::logic_error
	);

	EXPECT_THROW(
		RunTimeContainer({
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>(),
			std::make_shared<RunTimeImpl>()
		}),
		std::logic_error
	);
}

TEST(RunTimeContainerTest, equalsOperator) {
	const RunTimeContainer actual({
		std::make_shared<RunTimeImpl>(10),
		std::make_shared<RunTimeImpl>(11),
		std::make_shared<RunTimeImpl>(12),
		std::make_shared<RunTimeImpl>(13),
		std::make_shared<RunTimeImpl>(14),
		std::make_shared<RunTimeImpl>(15)
	});

	{
		const RunTimeContainer otherSameAsActaul({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_TRUE(actual == otherSameAsActaul);
	}

	{
		const RunTimeContainer other1({
			std::make_shared<RunTimeImpl>(0),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_FALSE(actual == other1);
	}

	{
		const RunTimeContainer other2({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(0),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_FALSE(actual == other2);
	}

	{
		const RunTimeContainer other3({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(0),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_FALSE(actual == other3);
	}

	{
		const RunTimeContainer other4({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(0),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_FALSE(actual == other4);
	}

	{
		const RunTimeContainer other5({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(0),
			std::make_shared<RunTimeImpl>(15)
		});

		EXPECT_FALSE(actual == other5);
	}

	{
		const RunTimeContainer other6({
			std::make_shared<RunTimeImpl>(10),
			std::make_shared<RunTimeImpl>(11),
			std::make_shared<RunTimeImpl>(12),
			std::make_shared<RunTimeImpl>(13),
			std::make_shared<RunTimeImpl>(14),
			std::make_shared<RunTimeImpl>(0)
		});

		EXPECT_FALSE(actual == other6);
	}
}

TEST(RunTimeContainerTest, size) {
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THAT(runTimes, SizeIs(ZoneHandler::getZoneCount()));
}

TEST(RunTimeContainerTest, id) {
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(IdType(i)));
	}
}

TEST(RunTimeContainerTest, at) {
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(runTimes.at(i).get(), Eq(next(runTimes.begin(), i)->second.get()));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////

#define RUN_TIME_DTO(i)  	RunTimeDTO().setMinutes(10 + i).setSeconds(20 + i)


TEST(RunTimeContainerTest, updateFromRunTimeDtoList) {
	const std::list<RunTimeDTO> runTimeDtoList {
		RUN_TIME_DTO(4).setId(0),
		RUN_TIME_DTO(5).setId(1),
		RUN_TIME_DTO(6).setId(2),
		RUN_TIME_DTO(7).setId(3),
		RUN_TIME_DTO(8).setId(4),
		RUN_TIME_DTO(9).setId(5)
	};

	std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory = std::make_shared<MockRunTimeFactory>();

	for (size_t i = 0; i < runTimeDtoList.size(); ++i) {
		mockRunTimeFactory->mockRunTimes.push_back(std::make_shared<StrictMock<MockRunTime>>());

		EXPECT_CALL(*mockRunTimeFactory->mockRunTimes.back(), updateFromRunTimeDto(*next(runTimeDtoList.begin(), i)))
			.Times(1);
	}

	RunTimeContainer runTimeContainer(mockRunTimeFactory);
	EXPECT_THAT(runTimeContainer.size(), Eq(6));

	runTimeContainer.updateFromRunTimeDtoList(runTimeDtoList);
	ASSERT_THAT(runTimeContainer.size(), Eq(runTimeDtoList.size()));

	for (size_t i = 0; i < runTimeDtoList.size(); ++i) {
		const IdType id = next(runTimeDtoList.begin(), i)->getId();

		ASSERT_NO_THROW(runTimeContainer.at(id));
		EXPECT_THAT(runTimeContainer.at(id).get(), Eq(mockRunTimeFactory->mockRunTimes[i].get()));
	}
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_zeroItem) {
	const std::list<RunTimeDTO> runTimeDtoList({
	});

	EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_oneItem) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO()
	});

	EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_lessThanExpectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_expectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	EXPECT_NO_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList));
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_moreThanExpectedCountOfItems) {
	const std::list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_expectedCountOfItemsWithWrongId) {
	{
		const std::list<RunTimeDTO> runTimeDtoList({
			RunTimeDTO().setId(5),
			RunTimeDTO().setId(4),
			RunTimeDTO().setId(3),
			RunTimeDTO().setId(2),
			RunTimeDTO().setId(1),
			RunTimeDTO().setId(0)
		});

		EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
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

		EXPECT_THROW(RunTimeContainer(std::make_shared<RunTimeFactory>()).updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, toRunTimeDtoList) {
	const std::initializer_list<RunTimeContainer::mapped_type> initializer {
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>(),
		std::make_shared<StrictMock<MockRunTime>>()
	};

	const std::list<RunTimeDTO> expected {
		RUN_TIME_DTO(0).setId(0),
		RUN_TIME_DTO(1).setId(1),
		RUN_TIME_DTO(2).setId(2),
		RUN_TIME_DTO(3).setId(3),
		RUN_TIME_DTO(4).setId(4),
		RUN_TIME_DTO(5).setId(5)
	};

	for (size_t i = 0; i < initializer.size(); i++) {
		MockRunTime* mockRunTime = dynamic_cast<MockRunTime*>(next(initializer.begin(), i)->get());

		ASSERT_THAT(mockRunTime, Not(IsNull()));
		EXPECT_CALL(*mockRunTime, toRunTimeDto())
			.Times(1)
			.WillOnce(Return(RUN_TIME_DTO(i)));
	}

	EXPECT_THAT(RunTimeContainer(initializer).toRunTimeDtoList(), ContainerEq(expected));
}
