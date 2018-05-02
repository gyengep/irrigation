/*
 * Program_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include "common.h"
#include "Logic/Program.h"
#include "Logic/Scheduler.h"
#include <gmock/gmock.h>


extern std::time_t toTime(int year, int month, int day, int hour, int min, int sec, bool dst);

TEST(Program, name) {
	Program program("");

	EXPECT_TRUE(program.getName().empty());

	const char* name = "AbcXyz";
	program.setName(name);
	EXPECT_EQ(name, program.getName());
}
/*
TEST(DISABLED_Program, isScheduled) {
	Program program("");
	std::time_t t;
	bool requestedResult;

	program.setSchedulerType(Program::SPECIFIED_DAYS);

	program.getSpecifiedScheduler().enableDay(WEDNESDAY, true);
	program.startTimes().add(StartTimeContainer::hourMin2StartTime(6, 5));

	for (int day = 19; day < 26; day++) {
		for (int hour = 0; hour < 24; hour++) {
			for (int min = 0; min < 60; min++) {
				for (int sec = 0; sec < 60; sec++) {
					t = toTime(2016, 9, day, hour, min, sec, true);
					requestedResult = (6 == hour) && (5 == min) && (0 == sec);
					requestedResult &= (21 == day);
					EXPECT_EQ(requestedResult, program.isScheduled(t));
				}
			}
		}
	}

	program.getSpecifiedScheduler().enableDay(SATURDAY, true);
	program.startTimes().add(StartTimeContainer::hourMin2StartTime(11, 10));
	program.startTimes().add(StartTimeContainer::hourMin2StartTime(20, 15));

	for (int day = 19; day < 26; day++) {
		for (int hour = 0; hour < 24; hour++) {
			for (int min = 0; min < 60; min++) {
				for (int sec = 0; sec < 60; sec++) {
					t = toTime(2016, 9, day, hour, min, sec, true);
					requestedResult =  (6 == hour) && (5 == min) && (0 == sec);
					requestedResult |= (11 == hour) && (10 == min) && (0 == sec);
					requestedResult |= (20 == hour) && (15 == min) && (0 == sec);
					requestedResult &= (21 == day) || (24 == day);
					EXPECT_EQ(requestedResult, program.isScheduled(t));
				}
			}
		}
	}
}
*/
