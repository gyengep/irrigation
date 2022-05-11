#include <gmock/gmock.h>
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/RunTimeContainer.h"
#include "Mocks/MockRunTime.h"
#include "Dto2Object/RunTimeListSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, defaultConstructor) {
/*
	const RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());

	ASSERT_THAT(runTimes, SizeIs(6));

	EXPECT_THAT(runTimes.at(0), Pointee(RunTimeImpl()));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTimeImpl()));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTimeImpl()));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTimeImpl()));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTimeImpl()));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTimeImpl()));
*/
}

TEST(RunTimeContainerTest, initializerConstructor) {
	const initializer_list<RunTimePtr> initializer {
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
		logic_error
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
		logic_error
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
		EXPECT_THAT(runTimes.at(i), Eq(next(runTimes.begin(), i)->second));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_zeroItem) {
	const list<RunTimeDTO> runTimeDtoList({
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_oneItem) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO()
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_lessItems) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_moreItems) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_wrongId_1) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO().setId(5),
		RunTimeDTO().setId(4),
		RunTimeDTO().setId(3),
		RunTimeDTO().setId(2),
		RunTimeDTO().setId(1),
		RunTimeDTO().setId(0)
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_wrongId_2) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO().setId(0),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, toRunTimeDtoList) {
	const std::list<RunTimeDTO> expected {
		RunTimeDTO().setId(0).setMinutes(10).setSeconds(20),
		RunTimeDTO().setId(1).setMinutes(11).setSeconds(21),
		RunTimeDTO().setId(2).setMinutes(12).setSeconds(22),
		RunTimeDTO().setId(3).setMinutes(13).setSeconds(23),
		RunTimeDTO().setId(4).setMinutes(14).setSeconds(24),
		RunTimeDTO().setId(5).setMinutes(15).setSeconds(25)
	};

	auto runTimeFactory = std::make_shared<MockRunTimeFactory>();

	for (int i = 0; i < 6; i++) {
		auto mockRunTime = std::make_shared<StrictMock<MockRunTime>>();

		EXPECT_CALL(*mockRunTime, toRunTimeDto())
			.Times(1)
			.WillOnce(Return(*std::next(expected.begin(), i)));

		runTimeFactory->mockRunTimes.push_back(mockRunTime);
	}

	EXPECT_THAT(RunTimeContainer(runTimeFactory).toRunTimeDtoList(), ContainerEq(expected));
}

