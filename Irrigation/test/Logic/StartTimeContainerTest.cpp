#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTimeContainer.h"
#include "MockStartTime.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerTest, defaultConstructor) {
	StartTimeContainer startTimes;
	EXPECT_THAT(startTimes.begin(), startTimes.end());
}

TEST(StartTimeContainerTest, initializerConstructor) {
	const initializer_list<StartTimeContainer::value_type> initializer {
		{10, shared_ptr<StartTime>(new StartTime())},
		{20, shared_ptr<StartTime>(new StartTime())},
		{15, shared_ptr<StartTime>(new StartTime())},
	};

	StartTimeContainer startTimes(initializer);
	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(*next(startTimes.begin(), i), Eq(*next(initializer.begin(), i)));
	}
}

TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;
	EXPECT_THAT(startTimes, SizeIs(0));

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
