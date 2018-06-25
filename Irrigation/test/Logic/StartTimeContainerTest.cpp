#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"

using namespace std;


class MockStartTime : public StartTime {
public:
	MockStartTime() {
		EXPECT_CALL(*this, destructorIsCalled()).Times(1);
	}

	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockStartTime() { destructorIsCalled(); }
};

static void insertToStartTimes(StartTimeContainer& startTimes, const vector<StartTimeContainer::value_type>& values) {
	for (unsigned i = 0; i < values.size(); ++i) {
		startTimes.insert(values[i].first, values[i].second);
	}
}

static void expectStartTimes(const StartTimeContainer& startTimes, const vector<StartTimeContainer::value_type>& required) {
	ASSERT_EQ(required.size(), startTimes.size());

	unsigned i = 0;
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it, ++i) {
		EXPECT_EQ(required[i], *it);
	}
}


TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;
	EXPECT_EQ(0, startTimes.size());

	startTimes.insert(0, new StartTime());
	EXPECT_EQ(1, startTimes.size());
}

TEST(StartTimeContainerTest, insert) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;

	for (auto& pair : required) {
		EXPECT_EQ(pair, startTimes.insert(pair.first, pair.second));
	}

	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, insertExisted) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_THROW(startTimes.insert(20, new MockStartTime()), AlreadyExistException);
}

TEST(StartTimeContainerTest, erase) {
	const vector<StartTimeContainer::value_type> startTimesToAdd {
		{10, new StartTime()},
		{20, new StartTime()},
		{30, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);
	ASSERT_EQ(startTimesToAdd.size(), startTimes.size());

	startTimes.erase(30);

	const vector<StartTimeContainer::value_type> required {
		{10, startTimesToAdd[0].second},
		{20, startTimesToAdd[1].second},
		{15, startTimesToAdd[3].second},
	};

	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, eraseInvalid) {
	const vector<StartTimeContainer::value_type> required {
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
	startTimes.insert(0, new MockStartTime());
	startTimes.erase(0);
}

TEST(StartTimeContainerTest, at) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	ASSERT_EQ(required.size(), startTimes.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		EXPECT_EQ(required[i].second, startTimes.at(required[i].first));
	}
}

TEST(StartTimeContainerTest, atConst) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	const StartTimeContainer& constStartTimes = startTimes;

	insertToStartTimes(startTimes, required);

	ASSERT_EQ(required.size(), constStartTimes.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		EXPECT_EQ(required[i].second, constStartTimes.at(required[i].first));
	}
}

TEST(StartTimeContainerTest, atInvalid) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_THROW(startTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerTest, atConstInvalid) {
	const vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	const StartTimeContainer& constStartTimes = startTimes;
	EXPECT_THROW(constStartTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerTest, destructed) {
	StartTimeContainer startTimes;
	startTimes.insert(0, new MockStartTime());
}

TEST(StartTimeContainerTest, sort) {

	const vector<StartTimeContainer::value_type> startTimesToAdd {
		{0, new StartTime(0, 0, 15)},
		{1, new StartTime(0, 0, 25)},
		{2, new StartTime(0, 0, 10)},
		{3, new StartTime(0, 0, 20)},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);

	startTimes.sort();

	const vector<StartTimeContainer::value_type> required {
		{2, startTimesToAdd[2].second},
		{0, startTimesToAdd[0].second},
		{3, startTimesToAdd[3].second},
		{1, startTimesToAdd[1].second},
	};

	expectStartTimes(startTimes, required);
}

