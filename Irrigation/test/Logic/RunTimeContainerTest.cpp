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
	const RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());

	ASSERT_THAT(runTimes, SizeIs(6));

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime()));
}

TEST(RunTimeContainerTest, initializerConstructor) {
	const initializer_list<RunTimePtr> initializer {
		std::make_shared<RunTime>(10),
		std::make_shared<RunTime>(11),
		std::make_shared<RunTime>(12),
		std::make_shared<RunTime>(13),
		std::make_shared<RunTime>(14),
		std::make_shared<RunTime>(15)
	};

	const RunTimeContainer runTimes(initializer);

	ASSERT_THAT(runTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(i));
		EXPECT_THAT(next(runTimes.begin(), i)->second.get(), next(initializer.begin(), i)->get());
	}
}

TEST(RunTimeContainerTest, initializerConstructorWithWrongInitializer) {
	EXPECT_THROW(
		RunTimeContainer({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14)
		}),
		logic_error
	);

	EXPECT_THROW(
		RunTimeContainer({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15),
			std::make_shared<RunTime>(16)
		}),
		logic_error
	);
}

TEST(RunTimeContainerTest, equalsOperator) {
	const RunTimeContainer actual({
		std::make_shared<RunTime>(10),
		std::make_shared<RunTime>(11),
		std::make_shared<RunTime>(12),
		std::make_shared<RunTime>(13),
		std::make_shared<RunTime>(14),
		std::make_shared<RunTime>(15)
	});

	{
		const RunTimeContainer otherSameAsActaul({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15)
		});

		EXPECT_TRUE(actual == otherSameAsActaul);
	}

	{
		const RunTimeContainer other1({
			std::make_shared<RunTime>(0),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15)
		});

		EXPECT_FALSE(actual == other1);
	}

	{
		const RunTimeContainer other2({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(0),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15)
		});

		EXPECT_FALSE(actual == other2);
	}

	{
		const RunTimeContainer other3({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(0),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15)
		});

		EXPECT_FALSE(actual == other3);
	}

	{
		const RunTimeContainer other4({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(0),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(15)
		});

		EXPECT_FALSE(actual == other4);
	}

	{
		const RunTimeContainer other5({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(0),
			std::make_shared<RunTime>(15)
		});

		EXPECT_FALSE(actual == other5);
	}

	{
		const RunTimeContainer other6({
			std::make_shared<RunTime>(10),
			std::make_shared<RunTime>(11),
			std::make_shared<RunTime>(12),
			std::make_shared<RunTime>(13),
			std::make_shared<RunTime>(14),
			std::make_shared<RunTime>(0)
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
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(i));
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
	RunTimeContainer runTimes(std::make_shared<RunTimeFactory>());
	EXPECT_THROW(runTimes.updateFromRunTimeDtoList(list<RunTimeDTO>()), IllegalArgumentException);
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

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_wrongId) {
	{
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

	{
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
}

///////////////////////////////////////////////////////////////////////////////

void testToRunTimeDtoList(const RunTimeListSample& runTimeListSample) {
	EXPECT_THAT(
		runTimeListSample.getContainerPtr()->toRunTimeDtoList(),
		Eq(runTimeListSample.getDtoList())
	);
}

TEST(RunTimeContainerTest, toRunTimeDtoList1) {
	testToRunTimeDtoList(RunTimeListSample1());
}

TEST(RunTimeContainerTest, toRunTimeDtoList2) {
	testToRunTimeDtoList(RunTimeListSample2());
}

TEST(RunTimeContainerTest, toRunTimeDtoList3) {
	testToRunTimeDtoList(RunTimeListSample3());
}

TEST(RunTimeContainerTest, toRunTimeDtoList4) {
	testToRunTimeDtoList(RunTimeListSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromRunTimeDtoList(shared_ptr<RunTimeContainer> runTimes, const RunTimeListSample& runTimeListSample) {
	EXPECT_THAT(runTimes, Pointee(Not(Eq(std::ref(*runTimeListSample.getContainerPtr())))));
	runTimes->updateFromRunTimeDtoList(runTimeListSample.getDtoList());
	EXPECT_THAT(runTimes, Pointee(Eq(std::ref(*runTimeListSample.getContainerPtr()))));
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList1) {
	shared_ptr<RunTimeContainer> runTimes = std::make_shared<RunTimeContainer>(std::make_shared<RunTimeFactory>());
	testUpdateFromRunTimeDtoList(runTimes, RunTimeListSample1());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList2) {
	testUpdateFromRunTimeDtoList(RunTimeListSample1().getContainerPtr(), RunTimeListSample2());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList3) {
	testUpdateFromRunTimeDtoList(RunTimeListSample2().getContainerPtr(), RunTimeListSample3());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList4) {
	testUpdateFromRunTimeDtoList(RunTimeListSample3().getContainerPtr(), RunTimeListSample4());
}
