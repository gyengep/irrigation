#include <gmock/gmock.h>
#include "Logic/Exceptions.h"
#include "Logic/Program.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/Scheduler.h"

using namespace std;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;


TEST(Program, name) {
	Program program;

	EXPECT_TRUE(program.getName().empty());

	const char* name = "AbcXyz";
	program.setName(name);
	EXPECT_EQ(name, program.getName());
}

TEST(Program, setSchedulerTypeInvalid) {
	Program program;

	SchedulerType invalidSchedulerType = (SchedulerType)(SPECIFIED_DAYS + 1);
	EXPECT_THROW(program.setSchedulerType(invalidSchedulerType), InvalidSchedulerException);
}

TEST(Program, getSchedulerType) {
	Program program;
	EXPECT_EQ(SPECIFIED_DAYS, program.getSchedulerType());
}

TEST(Program, getSpecifiedScheduler) {
	Program program;
	EXPECT_NE(nullptr, &program.getSpecifiedScheduler());
}

time_t toTime(int year, int month, int day, int hour, int min, int sec, bool dst) {

	EXPECT_TRUE(0 <= month && month < 12);
	EXPECT_TRUE(1 <= day && day < 32);
	EXPECT_TRUE(0 <= hour && hour < 24);
	EXPECT_TRUE(0 <= min && min < 60);
	EXPECT_TRUE(0 <= sec && sec < 60);

	tm tm;

	memset(&tm, 0, sizeof(tm));

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	tm.tm_isdst = dst ? 1 : 0;

	return mktime(&tm);
}

class MockScheduler : public Scheduler {
public:
	MOCK_CONST_METHOD1(isDayScheduled, bool(const tm&));
};

class MockSchedulerFactory : public SchedulerFactory {
public:
	virtual Scheduler* createScheduler(SchedulerType schedulerType) const {
		return new MockScheduler();
	}
};

TEST(Program, isScheduled1) {
	Program program(new MockSchedulerFactory());
	std::time_t t;

	const MockScheduler& mockScheduler = dynamic_cast<const MockScheduler&>(program.getCurrentScheduler());
	ON_CALL(mockScheduler, isDayScheduled(_)).WillByDefault(Return(false));
	EXPECT_CALL(mockScheduler, isDayScheduled(_)).Times(AnyNumber());

	program.getStartTimes().insert(0, new StartTime(4, 0, 0));
	program.getStartTimes().insert(1, new StartTime(6, 0, 0));
	program.getStartTimes().insert(2, new StartTime(6, 30, 0));
	program.getStartTimes().insert(3, new StartTime(20, 15, 0));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				t = toTime(2018, 5, 27, hour, min, sec, true);
				EXPECT_FALSE(program.isScheduled(t));
			}
		}
	}
}

TEST(Program, isScheduled2) {
	Program program(new MockSchedulerFactory());
	std::time_t t;

	const MockScheduler& mockScheduler = dynamic_cast<const MockScheduler&>(program.getCurrentScheduler());
	ON_CALL(mockScheduler, isDayScheduled(_)).WillByDefault(Return(true));
	EXPECT_CALL(mockScheduler, isDayScheduled(_)).Times(AnyNumber());

	program.getStartTimes().insert(0, new StartTime(4, 0, 0));
	program.getStartTimes().insert(1, new StartTime(6, 0, 0));
	program.getStartTimes().insert(2, new StartTime(6, 30, 0));
	program.getStartTimes().insert(3, new StartTime(20, 15, 0));

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				t = toTime(2018, 5, 27, hour, min, sec, true);

				bool requestedResult = false;
				requestedResult |= (hour == 4 && min == 0 && sec == 0);
				requestedResult |= (hour == 6 && min == 0 && sec == 0);
				requestedResult |= (hour == 6 && min == 30 && sec == 0);
				requestedResult |= (hour == 20 && min == 15 && sec == 0);

				EXPECT_EQ(requestedResult, program.isScheduled(t));
			}
		}
	}
}

