#include <gmock/gmock.h>
#include "Logic/StartTimeContainer.h"


using ::testing::Invoke;
using ::testing::Return;



TEST(StartTimeContainerTest, size) {
	StartTimeContainer startTimes;
	EXPECT_EQ(0, startTimes.size());

	startTimes.insert(std::make_pair(0, new StartTime()));
	EXPECT_EQ(1, startTimes.size());
}

TEST(StartTimeContainerTest, accessValid) {
	StartTimeContainer startTimes;

	startTimes.insert(std::make_pair(10, new StartTime()));
	startTimes.insert(std::make_pair(15, new StartTime()));
	startTimes.insert(std::make_pair(20, new StartTime()));

	EXPECT_NO_THROW(startTimes.at(10));
	EXPECT_NO_THROW(startTimes.at(15));
	EXPECT_NO_THROW(startTimes.at(20));
}

TEST(StartTimeContainerTest, accessInvalid) {
	StartTimeContainer startTimes;

	startTimes.insert(std::make_pair(20, new StartTime()));
	startTimes.insert(std::make_pair(15, new StartTime()));
	startTimes.insert(std::make_pair(10, new StartTime()));

	EXPECT_THROW(startTimes.at(6), InvalidStartTimeIdException);
}

TEST(StartTimeContainerTest, id) {
	StartTimeContainer startTimes;

	startTimes.insert(std::make_pair(10, new StartTime()));
	startTimes.insert(std::make_pair(15, new StartTime()));
	startTimes.insert(std::make_pair(20, new StartTime()));

	std::vector<unsigned> requredIds {10, 15, 20};
	ASSERT_EQ(requredIds.size(), startTimes.size());

	unsigned i = 0;
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it, ++i) {
		EXPECT_EQ(requredIds[i], it->first);
	}
}

TEST(StartTimeContainerTest, getAtConst) {
	StartTimeContainer startTimes;
	std::vector<StartTime*> startTimePtrs { new StartTime(), new StartTime(), new StartTime()};

	startTimes.insert(std::make_pair(10, startTimePtrs[0]));
	startTimes.insert(std::make_pair(20, startTimePtrs[1]));
	startTimes.insert(std::make_pair(15, startTimePtrs[2]));

	{
		const StartTimeContainer& constStartTimes = startTimes;

		EXPECT_EQ(startTimePtrs[0], constStartTimes.at(10));
		EXPECT_EQ(startTimePtrs[1], constStartTimes.at(20));
		EXPECT_EQ(startTimePtrs[2], constStartTimes.at(15));
	}
}

TEST(StartTimeContainerTest, getAt) {
	StartTimeContainer startTimes;
	std::vector<StartTime*> startTimePtrs { new StartTime(), new StartTime(), new StartTime()};

	startTimes.insert(std::make_pair(10, startTimePtrs[0]));
	startTimes.insert(std::make_pair(20, startTimePtrs[1]));
	startTimes.insert(std::make_pair(15, startTimePtrs[2]));

	EXPECT_EQ(startTimePtrs[0], startTimes.at(10));
	EXPECT_EQ(startTimePtrs[1], startTimes.at(20));
	EXPECT_EQ(startTimePtrs[2], startTimes.at(15));
}

TEST(StartTimeContainerTest, insert) {
	StartTimeContainer startTimes;
	StartTime* startTime = new StartTime();
	startTimes.insert(std::make_pair(10, startTime));

	EXPECT_EQ(10, startTimes.begin()->first);
	EXPECT_EQ(startTime, startTimes.begin()->second);
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

TEST(StartTimeContainerTest, sort) {
	StartTimeContainer startTimes;

	std::vector<std::pair<unsigned, unsigned>> idAndValue {{0, 15}, {1, 10}, {2, 20}, {3, 18}};

	for (unsigned i = 0; i < idAndValue.size(); ++i) {
		StartTime* startTime = new StartTime();
		startTime->setValue(idAndValue[i].second);
		startTimes.insert(std::make_pair(idAndValue[i].first, startTime));
	}

	startTimes.sort();

	std::vector<std::pair<unsigned, unsigned>> requredIdAndValue {{1, 10}, {0, 15}, {3, 18}, {2, 20}};
	ASSERT_EQ(requredIdAndValue.size(), startTimes.size());

	unsigned i = 0;
	for (auto it = startTimes.begin(); it != startTimes.end(); ++it, ++i) {
		EXPECT_EQ(requredIdAndValue[i].first, it->first);
		EXPECT_EQ(requredIdAndValue[i].second, it->second->getValue());
	}
}

