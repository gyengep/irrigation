#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTimeContainer.h"
#include "MockStartTime.h"


using namespace std;

typedef vector<pair<const IdType, StartTime*>> IdTypeStartTimeVector;

///////////////////////////////////////////////////////////////////////////////

static void insertToStartTimes(StartTimeContainer& startTimes, const IdTypeStartTimeVector& values) {
	for (unsigned i = 0; i < values.size(); ++i) {
		startTimes.insert(values[i].first, unique_ptr<StartTime>(values[i].second));
	}
}

static void expectStartTimes(const StartTimeContainer& startTimes, const IdTypeStartTimeVector& required) {
	ASSERT_EQ(required.size(), startTimes.size());

	unsigned i = 0;
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it, ++i) {
		EXPECT_EQ(required[i].first, it->first);
		EXPECT_EQ(required[i].second, it->second.get());
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;
	EXPECT_EQ(0, startTimes.size());

	startTimes.insert(0, unique_ptr<StartTime>(new StartTime()));
	EXPECT_EQ(1, startTimes.size());
}

TEST(StartTimeContainerTest, insert) {
	const IdTypeStartTimeVector required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;

	for (auto& requiredPair : required) {
		StartTimeContainer::value_type& insertedPair =
				startTimes.insert(requiredPair.first, unique_ptr<StartTime>(requiredPair.second));

		EXPECT_EQ(requiredPair.first, insertedPair.first);
		EXPECT_EQ(requiredPair.second, insertedPair.second.get());
	}

	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, insertExisted) {
	const IdTypeStartTimeVector required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_THROW(startTimes.insert(20, unique_ptr<StartTime>(new MockStartTime())), AlreadyExistException);
}

TEST(StartTimeContainerTest, erase) {
	const IdTypeStartTimeVector startTimesToAdd {
		{10, new StartTime()},
		{20, new StartTime()},
		{30, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);
	ASSERT_EQ(startTimesToAdd.size(), startTimes.size());

	startTimes.erase(30);

	const IdTypeStartTimeVector required {
		{10, startTimesToAdd[0].second},
		{20, startTimesToAdd[1].second},
		{15, startTimesToAdd[3].second},
	};

	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, eraseInvalid) {
	const IdTypeStartTimeVector required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_EQ(required.size(), startTimes.size());
	EXPECT_THROW(startTimes.erase(30), NoSuchElementException);
	EXPECT_EQ(required.size(), startTimes.size());
}

TEST(StartTimeContainerTest, eraseDestructed) {
	StartTimeContainer startTimes;
	startTimes.insert(0, unique_ptr<StartTime>(new MockStartTime()));
	startTimes.erase(0);
}

TEST(StartTimeContainerTest, at) {
	const IdTypeStartTimeVector required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	ASSERT_EQ(required.size(), startTimes.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		const IdType& requiredId = required[i].first;
		const StartTime* requiredPtr = required[i].second;

		EXPECT_EQ(requiredPtr, startTimes.at(requiredId).get());
	}
}

TEST(StartTimeContainerTest, atInvalid) {
	const IdTypeStartTimeVector required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_THROW(startTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerTest, destroyed) {
	StartTimeContainer startTimes;
	startTimes.insert(0, unique_ptr<StartTime>(new MockStartTime()));
}

TEST(StartTimeContainerTest, sort) {

	const IdTypeStartTimeVector startTimesToAdd {
		{0, new StartTime(0, 0, 15)},
		{1, new StartTime(0, 0, 25)},
		{2, new StartTime(0, 0, 10)},
		{3, new StartTime(0, 0, 20)},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);

	startTimes.sort();

	const IdTypeStartTimeVector required {
		{2, startTimesToAdd[2].second},
		{0, startTimesToAdd[0].second},
		{3, startTimesToAdd[3].second},
		{1, startTimesToAdd[1].second},
	};

	expectStartTimes(startTimes, required);
}
