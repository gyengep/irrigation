#include <gmock/gmock.h>
#include "Logic/RunTimeContainer.h"



TEST(RunTimeContainerTest, size) {
	RunTimeContainer runTimes;
	EXPECT_EQ(ZONE_COUNT, runTimes.size());
}

TEST(RunTimeContainerTest, accessValid) {
	RunTimeContainer runTimes;

	for (unsigned i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(runTimes.at(i));
	}
}

TEST(RunTimeContainerTest, accessInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZONE_COUNT), InvalidRunTimeIdException);
}

TEST(RunTimeContainerTest, id) {
	RunTimeContainer runTimes;

	unsigned id = 0;
	for (auto it = runTimes.begin(); it != runTimes.end(); ++it, ++id) {
		EXPECT_EQ(id, it->first);
	}
}

TEST(RunTimeContainerTest, getAtConst) {
	const RunTimeContainer runTimes;
	std::vector<RunTime*> runTimePtrs;

	for (auto it = runTimes.begin(); it != runTimes.end(); ++it) {
		runTimePtrs.push_back(it->second);
	}

	for (unsigned i = 0; i < runTimes.size(); i++) {
		EXPECT_EQ(runTimePtrs[i], runTimes.at(i));
	}
}

TEST(RunTimeContainerTest, getAt) {
	RunTimeContainer runTimes;
	std::vector<RunTime*> runTimePtrs;

	for (auto it = runTimes.begin(); it != runTimes.end(); ++it) {
		runTimePtrs.push_back(it->second);
	}

	for (unsigned i = 0; i < runTimes.size(); i++) {
		EXPECT_EQ(runTimePtrs[i], runTimes.at(i));
	}
}


class MockRunTime : public RunTime {
public:
	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockRunTime() { destructorIsCalled(); }
};


class MockRunTimeFactory : public RunTimeFactory {
public:
	virtual RunTime* createRunTime() {
		return new MockRunTime();
	}
};

TEST(RunTimeContainerTest, destructed) {
	RunTimeContainer runTimes(new MockRunTimeFactory());

	for (unsigned i = 0; i < runTimes.size(); i++) {
		MockRunTime* mockRunTime = dynamic_cast<MockRunTime*>(runTimes.at(i));

		ASSERT_NE(nullptr, mockRunTime);
		EXPECT_CALL(*mockRunTime, destructorIsCalled()).Times(1);
	}
}
