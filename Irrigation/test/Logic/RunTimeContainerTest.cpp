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
	const RunTimeContainer runTimes;

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime()));
}

TEST(RunTimeContainerTest, initializerConstructor) {
	const RunTimeContainer runTimes{ 10, 11, 12, 13, 14, 15 };

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime(10)));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime(11)));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime(12)));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime(13)));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime(14)));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime(15)));
}

TEST(RunTimeContainerTest, initializerConstructorWithWrongInitializer) {
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14 }), logic_error);
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14, 15, 16 }), logic_error);
}
/*
TEST(RunTimeContainerTest, copyOperator) {
	const RunTimeContainer source{ 10, 11, 12, 13, 14, 15 };
	RunTimeContainer runTimes;
	runTimes = source;

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime(10)));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime(11)));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime(12)));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime(13)));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime(14)));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime(15)));
}
*/
TEST(RunTimeContainerTest, equalsOperator) {
	EXPECT_TRUE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 21, 22, 23, 24, 25 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 21, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 22, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 23, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 24, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 14, 25 }));
}

TEST(RunTimeContainerTest, size) {
	RunTimeContainer runTimes;
	EXPECT_THAT(runTimes, SizeIs(ZoneHandler::getZoneCount()));
}

TEST(RunTimeContainerTest, id) {
	RunTimeContainer runTimes;

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(i));
	}
}

TEST(RunTimeContainerTest, at) {
	RunTimeContainer runTimes;

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(runTimes.at(i), Eq(next(runTimes.begin(), i)->second));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

TEST(RunTimeContainerTest, destroyed) {
	RunTimeContainer runTimes(unique_ptr<RunTimeFactory>(new MockRunTimeFactory()));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_zeroItem) {
	EXPECT_THROW(RunTimeContainer().updateFromRunTimeDtoList(list<RunTimeDTO>()), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_oneItem) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO()
	});

	EXPECT_THROW(RunTimeContainer().updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList_lessItems) {
	const list<RunTimeDTO> runTimeDtoList({
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO(),
		RunTimeDTO()
	});

	EXPECT_THROW(RunTimeContainer().updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
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

	EXPECT_THROW(RunTimeContainer().updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
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

		RunTimeContainer runTimeContainer;
		EXPECT_THROW(runTimeContainer.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
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

		RunTimeContainer runTimeContainer;
		EXPECT_THROW(runTimeContainer.updateFromRunTimeDtoList(runTimeDtoList), IllegalArgumentException);
	}
}

///////////////////////////////////////////////////////////////////////////////

void testToRunTimeDtoList(const RunTimeListSample& runTimeListSample) {
	EXPECT_THAT(
		runTimeListSample.getContainer()->toRunTimeDtoList(),
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

void testUpdateFromRunTimeDtoList(shared_ptr<RunTimeContainer> runTimeContainer, const RunTimeListSample& runTimeListSample) {
	EXPECT_THAT(runTimeContainer, Not(Pointee(*runTimeListSample.getContainer())));
	runTimeContainer->updateFromRunTimeDtoList(runTimeListSample.getDtoList());
	EXPECT_THAT(runTimeContainer, Pointee(*runTimeListSample.getContainer()));
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList1) {
	shared_ptr<RunTimeContainer> runTimeContainer = shared_ptr<RunTimeContainer>(new RunTimeContainer());
	testUpdateFromRunTimeDtoList(runTimeContainer, RunTimeListSample1());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList2) {
	shared_ptr<RunTimeContainer> runTimeContainer = RunTimeListSample1().getContainer();
	testUpdateFromRunTimeDtoList(runTimeContainer, RunTimeListSample2());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList3) {
	shared_ptr<RunTimeContainer> runTimeContainer = RunTimeListSample2().getContainer();
	testUpdateFromRunTimeDtoList(runTimeContainer, RunTimeListSample3());
}

TEST(RunTimeContainerTest, updateFromRunTimeDtoList4) {
	shared_ptr<RunTimeContainer> runTimeContainer = RunTimeListSample3().getContainer();
	testUpdateFromRunTimeDtoList(runTimeContainer, RunTimeListSample4());
}
