#include <gmock/gmock.h>
#include "Logic/Exceptions.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"



static void insertToStartTimes(StartTimeContainer& startTimes, const std::vector<StartTimeContainer::value_type>& values) {
	for (unsigned i = 0; i < values.size(); ++i) {
		startTimes.insert(values[i]);
	}
}

static void expectStartTimes(const StartTimeContainer& startTimes, const std::vector<StartTimeContainer::value_type>& required) {
	ASSERT_EQ(required.size(), startTimes.size());

	unsigned i = 0;
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it, ++i) {
		EXPECT_EQ(required[i], *it);
	}
}


TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;
	EXPECT_EQ(0, startTimes.size());

	startTimes.insert(std::make_pair(0, new StartTime()));
	EXPECT_EQ(1, startTimes.size());
}

TEST(StartTimeContainerTest, insert) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);
	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, erase) {
	const std::vector<StartTimeContainer::value_type> startTimesToAdd {
		{10, new StartTime()},
		{20, new StartTime()},
		{30, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);
	ASSERT_EQ(startTimesToAdd.size(), startTimes.size());

	startTimes.erase(30);

	const std::vector<StartTimeContainer::value_type> required {
		{10, startTimesToAdd[0].second},
		{20, startTimesToAdd[1].second},
		{15, startTimesToAdd[3].second},
	};

	expectStartTimes(startTimes, required);
}

TEST(StartTimeContainerTest, eraseInvalid) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_EQ(required.size(), startTimes.size());
	EXPECT_THROW(startTimes.erase(30), InvalidStartTimeIdException);
	EXPECT_EQ(required.size(), startTimes.size());
}

TEST(StartTimeContainerTest, sort) {

	const std::vector<StartTimeContainer::value_type> startTimesToAdd {
		{0, new StartTime(15)},
		{1, new StartTime(25)},
		{2, new StartTime(10)},
		{3, new StartTime(20)},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, startTimesToAdd);

	startTimes.sort();

	const std::vector<StartTimeContainer::value_type> required {
		{2, startTimesToAdd[2].second},
		{0, startTimesToAdd[0].second},
		{3, startTimesToAdd[3].second},
		{1, startTimesToAdd[1].second},
	};

	expectStartTimes(startTimes, required);
}


TEST(StartTimeContainerTest, accessValid) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_NO_THROW(startTimes.at(10));
	EXPECT_NO_THROW(startTimes.at(15));
	EXPECT_NO_THROW(startTimes.at(20));
}

TEST(StartTimeContainerTest, accessInvalid) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{15, new StartTime()},
		{20, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	EXPECT_THROW(startTimes.at(6), InvalidStartTimeIdException);
}

TEST(StartTimeContainerTest, getAtConst) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	{
		const StartTimeContainer& constStartTimes = startTimes;

		ASSERT_EQ(required.size(), constStartTimes.size());
		for (unsigned i = 0; i < required.size(); ++i) {
			EXPECT_EQ(required[i].second, constStartTimes.at(required[i].first));
		}
	}
}

TEST(StartTimeContainerTest, getAt) {
	const std::vector<StartTimeContainer::value_type> required {
		{10, new StartTime()},
		{20, new StartTime()},
		{15, new StartTime()},
	};

	StartTimeContainer startTimes;
	insertToStartTimes(startTimes, required);

	ASSERT_EQ(required.size(), startTimes.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		EXPECT_EQ(required[i].second, startTimes.at(required[i].first));
	}
}


class MockStartTime : public StartTime {
public:
	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockStartTime() { destructorIsCalled(); }
};


TEST(StartTimeContainerTest, destructed) {
	MockStartTime* mockStartTime = new MockStartTime();
	EXPECT_CALL(*mockStartTime, destructorIsCalled()).Times(1);

	{
		StartTimeContainer startTimes;
		startTimes.insert(std::make_pair(0, mockStartTime));
	}
}

TEST(StartTimeContainerTest, eraseDestructed) {
	MockStartTime* mockStartTime = new MockStartTime();
	EXPECT_CALL(*mockStartTime, destructorIsCalled()).Times(1);

	StartTimeContainer startTimes;
	startTimes.insert(std::make_pair(0, mockStartTime));
	startTimes.erase(0);
}

