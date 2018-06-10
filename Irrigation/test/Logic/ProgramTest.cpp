#include <gmock/gmock.h>
#include "Logic/Exceptions.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"

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

	int specifiedDaysAsInt = static_cast<int>(SchedulerType::SPECIFIED_DAYS);
	SchedulerType invalidSchedulerType = static_cast<SchedulerType>(specifiedDaysAsInt + 1);

	EXPECT_THROW(program.setSchedulerType(invalidSchedulerType), InvalidSchedulerTypeException);
}

TEST(Program, getSchedulerType) {
	Program program;
	EXPECT_EQ(SchedulerType::SPECIFIED_DAYS, program.getSchedulerType());
}

TEST(Program, getSpecifiedScheduler) {
	Program program;
	EXPECT_NE(nullptr, &program.getSpecifiedScheduler());
}

TEST(Program, getProgramDTO) {
	Program program;
	program.setName("Abcdefg");
	program.setSchedulerType(SchedulerType::SPECIFIED_DAYS);

	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::MONDAY, true);
	program.getSpecifiedScheduler().enableDay(SpecifiedScheduler::FRIDAY, true);

	program.getRunTimes().at(0)->setValue(20);
	program.getRunTimes().at(1)->setValue(21);
	program.getRunTimes().at(2)->setValue(22);
	program.getRunTimes().at(3)->setValue(23);
	program.getRunTimes().at(4)->setValue(24);
	program.getRunTimes().at(5)->setValue(25);

	program.getStartTimes().insert(100, new StartTime(1, 0, 0));
	program.getStartTimes().insert(101, new StartTime(1, 0, 1));
	program.getStartTimes().insert(102, new StartTime(1, 0, 2));

	const ProgramDTO expectedProgramDTO("Abcdefg", "specified",
			SpecifiedSchedulerDTO(new list<bool>({ false, true, false, false, false, true, false})),
			new list<RunTimeDTO>({
				RunTimeDTO(20).setId(0),
				RunTimeDTO(21).setId(1),
				RunTimeDTO(22).setId(2),
				RunTimeDTO(23).setId(3),
				RunTimeDTO(24).setId(4),
				RunTimeDTO(25).setId(5)
			}),
			new list<StartTimeDTO>({
				StartTimeDTO(3600).setId(100),
				StartTimeDTO(3601).setId(101),
				StartTimeDTO(3602).setId(102)
			})
		);

	EXPECT_EQ(expectedProgramDTO, program.getProgramDTO());
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

class MockScheduler : public SpecifiedScheduler {
public:
	MOCK_CONST_METHOD1(isDayScheduled, bool(const tm&));
};

class MockSchedulerFactory : public SchedulerFactory {
public:
	virtual SpecifiedScheduler* createSpecifiedSheduler() const {
		return new MockScheduler();
	}
};

TEST(Program, isScheduled1) {
	MockSchedulerFactory schedulerFactory;
	Program program(schedulerFactory);
	time_t t;

	program.getCurrentScheduler();
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
	MockSchedulerFactory schedulerFactory;
	Program program(schedulerFactory);
	time_t t;

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

