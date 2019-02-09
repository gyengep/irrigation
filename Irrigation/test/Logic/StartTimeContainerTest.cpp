#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTimeContainer.h"
#include "Mocks/MockStartTime.h"
#include "Dto2Object/StartTimeListSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerTest, defaultConstructor) {
	StartTimeContainer startTimes;
	EXPECT_THAT(startTimes.begin(), startTimes.end());
	EXPECT_THAT(startTimes, SizeIs(0));
}

TEST(StartTimeContainerTest, initializerConstructor) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime(1, 10))},
		{20, shared_ptr<StartTime>(new StartTime(2, 20))},
		{15, shared_ptr<StartTime>(new StartTime(3, 30))},
	};
	const StartTimeContainer startTimes(initializer);

	ASSERT_THAT(startTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(startTimes.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(startTimes.begin(), i)->second.get(), Eq(next(initializer.begin(), i)->second.get()));
		EXPECT_THAT(next(startTimes.begin(), i)->second.get(), Pointee(*next(initializer.begin(), i)->second.get()));
	}
}

TEST(StartTimeContainerTest, copyConstructor) {
	const StartTimeContainer startTimes1({
		{10, shared_ptr<StartTime>(new StartTime(4, 50))},
		{20, shared_ptr<StartTime>(new StartTime(5, 51))},
		{15, shared_ptr<StartTime>(new StartTime(6, 52))},
	});
	const StartTimeContainer startTimes2(startTimes1);

	ASSERT_THAT(startTimes2, SizeIs(startTimes1.size()));

	for (size_t i = 0; i < startTimes1.size(); ++i) {
		EXPECT_THAT(next(startTimes2.begin(), i)->first, Eq(next(startTimes1.begin(), i)->first));
		EXPECT_THAT(next(startTimes2.begin(), i)->second.get(), Ne(next(startTimes1.begin(), i)->second.get()));
		EXPECT_THAT(next(startTimes2.begin(), i)->second.get(), Pointee(*next(startTimes1.begin(), i)->second.get()));
	}
}

TEST(StartTimeContainerTest, equalsOperator) {
	const StartTime startTime1(10, 20);
	const StartTime startTime2(15, 20);

	{
		StartTimeContainer container1;
		StartTimeContainer container2;

		EXPECT_TRUE(container1 == container2);
		EXPECT_TRUE(container2 == container1);

		container1.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);

		container2.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		EXPECT_TRUE(container1 == container2);
		EXPECT_TRUE(container2 == container1);
	}

	{
		StartTimeContainer container1;
		StartTimeContainer container2;

		container1.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		container2.insert(1001, shared_ptr<StartTime>(new StartTime(startTime1)));
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		StartTimeContainer container1;
		StartTimeContainer container2;

		container1.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		container2.insert(1000, shared_ptr<StartTime>(new StartTime(startTime2)));
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		StartTimeContainer container1;
		StartTimeContainer container2;

		container1.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		container1.insert(1001, shared_ptr<StartTime>(new StartTime(startTime2)));
		container2.insert(1001, shared_ptr<StartTime>(new StartTime(startTime2)));
		container2.insert(1000, shared_ptr<StartTime>(new StartTime(startTime1)));
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}
}

TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;

	startTimes.insert(0, shared_ptr<StartTime>(new StartTime()));
	EXPECT_THAT(startTimes, SizeIs(1));
}

TEST(StartTimeContainerTest, insert) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
	};

	StartTimeContainer startTimes;

	for (const auto& value : initializer) {
		startTimes.insert(value.first, value.second);
	}

	EXPECT_THAT(startTimes, ElementsAreArray(initializer));
}

TEST(StartTimeContainerTest, insertExisted) {
	StartTimeContainer startTimes({
		{10, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
	});

	EXPECT_THROW(startTimes.insert(20, shared_ptr<StartTime>(new MockStartTime())), AlreadyExistException);
}

TEST(StartTimeContainerTest, erase) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
		{30, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
	};

	StartTimeContainer startTimes(initializer);

	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
	EXPECT_NO_THROW(startTimes.erase(30));
	EXPECT_THAT(startTimes, SizeIs(initializer.size() - 1));

	EXPECT_THAT(startTimes,
			ElementsAre(
				*next(initializer.begin(), 0),
				*next(initializer.begin(), 1),
				*next(initializer.begin(), 3)
			));
}

TEST(StartTimeContainerTest, eraseInvalid) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
	};

	StartTimeContainer startTimes(initializer);

	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
	EXPECT_THROW(startTimes.erase(30), NoSuchElementException);
	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
}

TEST(StartTimeContainerTest, eraseDestroy) {
	StartTimeContainer startTimes;
	startTimes.insert(0, shared_ptr<StartTime>(new MockStartTime()));
	startTimes.erase(0);
}

TEST(StartTimeContainerTest, at) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
	};

	StartTimeContainer startTimes(initializer);

	ASSERT_THAT(startTimes, SizeIs(initializer.size()));
	for (unsigned i = 0; i < initializer.size(); ++i) {
		const StartTimeContainer::value_type& value = *next(initializer.begin(), i);
		const StartTimeContainer::key_type& requiredKey = value.first;
		const StartTimeContainer::mapped_type& requiredValue = value.second;

		EXPECT_THAT(startTimes.at(requiredKey), Eq(requiredValue));
	}
}

TEST(StartTimeContainerTest, atInvalid) {
	StartTimeContainer startTimes({
		{10, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
	});

	EXPECT_THROW(startTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerTest, destroyed) {
	StartTimeContainer startTimes;
	startTimes.insert(0, shared_ptr<StartTime>(new MockStartTime()));
}

TEST(StartTimeContainerTest, sort) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{0, shared_ptr<StartTime>(new StartTime(0, 15))},
		{1, shared_ptr<StartTime>(new StartTime(0, 25))},
		{2, shared_ptr<StartTime>(new StartTime(0, 10))},
		{3, shared_ptr<StartTime>(new StartTime(0, 20))},
	};

	StartTimeContainer startTimes(initializer);

	startTimes.sort();

	EXPECT_THAT(startTimes,
			ElementsAre(
				*next(initializer.begin(), 2),
				*next(initializer.begin(), 0),
				*next(initializer.begin(), 3),
				*next(initializer.begin(), 1)
			));
}

///////////////////////////////////////////////////////////////////////////////

void testToStartTimeDtoList(const StartTimeListSample& startTimeListSample) {
	const shared_ptr<StartTimeContainer> startTimeContainer = startTimeListSample.getContainer();
	const list<StartTimeDTO>& expectedStartTimeDtoList = startTimeListSample.getDtoList();
	EXPECT_THAT(startTimeContainer->toStartTimeDtoList(), Eq(expectedStartTimeDtoList));
}

TEST(StartTimeContainerTest, toStartTimeDtoList_empty) {
	testToStartTimeDtoList(StartTimeListSample1());
}

TEST(StartTimeContainerTest, toStartTimeDtoList_oneItem) {
	testToStartTimeDtoList(StartTimeListSample2());
}

TEST(StartTimeContainerTest, toStartTimeDtoList_moreItem1) {
	testToStartTimeDtoList(StartTimeListSample3());
}

TEST(StartTimeContainerTest, toStartTimeDtoList_moreItem2) {
	testToStartTimeDtoList(StartTimeListSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromStartTimeDtoList(shared_ptr<StartTimeContainer> startTimeContainer, const StartTimeListSample& startTimeListSample) {
	EXPECT_THAT(startTimeContainer, Not(Pointee(*startTimeListSample.getContainer())));
	startTimeContainer->updateFromStartTimeDtoList(startTimeListSample.getDtoList());
	EXPECT_THAT(startTimeContainer, Pointee(*startTimeListSample.getContainer()));
}

TEST(StartTimeContainerTest, updateFromStartTimeDtoList_empty) {
	shared_ptr<StartTimeContainer> startTimeContainer = StartTimeListSample2().getContainer();
	testUpdateFromStartTimeDtoList(startTimeContainer, StartTimeListSample1());
}

TEST(StartTimeContainerTest, updateFromStartTimeDtoList_oneItem) {
	shared_ptr<StartTimeContainer> startTimeContainer = StartTimeListSample3().getContainer();
	testUpdateFromStartTimeDtoList(startTimeContainer, StartTimeListSample2());
}

TEST(StartTimeContainerTest, updateFromStartTimeDtoList_moreItem1) {
	shared_ptr<StartTimeContainer> startTimeContainer = StartTimeListSample1().getContainer();
	testUpdateFromStartTimeDtoList(startTimeContainer, StartTimeListSample3());
}

TEST(StartTimeContainerTest, updateFromStartTimeDtoList_moreItem2) {
	shared_ptr<StartTimeContainer> startTimeContainer = StartTimeListSample2().getContainer();
	testUpdateFromStartTimeDtoList(startTimeContainer, StartTimeListSample4());
}
