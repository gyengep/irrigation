/*
 * Program_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include "gtest/gtest.h"

#include "exception.h"
#include "Program.h"


TEST(Program, SetName) {
	Program program;
	const char* name = "AbcXyz";

	EXPECT_EQ("", program.name);

	program.setName(name);
	EXPECT_EQ(name, program.name);
}

TEST(Program, GetName) {
	Program program;
	const char* name = "AbcXyz";

	program.setName(name);
	EXPECT_EQ(name, program.getName());
}

TEST(Program, SetRunTime) {
	Program program;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		program.setRunTime(i, i * 10);
	}

	EXPECT_THROW(program.setRunTime(i, 0), invalid_id);

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i, program.runTimes[i].first);
		EXPECT_EQ(i * 10, program.runTimes[i].second);
	}
}

TEST(Program, GetRunTime) {
	Program program;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(program.getRunTime(i));
	}

	EXPECT_THROW(program.getRunTime(i), invalid_id);

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(0, program.getRunTime(i));
		program.setRunTime(i, i * 10);
	}

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i * 10, program.getRunTime(i));
	}

}

TEST(Program, AddStartTime) {
	Program program;

	EXPECT_EQ(0, program.startTimes.size());

	EXPECT_EQ(0, program.addStartTime(10));
	EXPECT_EQ(1, program.startTimes.size());
	EXPECT_EQ(0, program.startTimes.back().first);
	EXPECT_EQ(10, program.startTimes.back().second);

	EXPECT_EQ(1, program.addStartTime(20));
	EXPECT_EQ(2, program.startTimes.size());
	EXPECT_EQ(1, program.startTimes.back().first);
	EXPECT_EQ(20, program.startTimes.back().second);
}

TEST(Program, DeleteStartTime) {
	Program program;

	program.addStartTime(10);
	program.addStartTime(20);
	program.addStartTime(30);
	program.addStartTime(40);

	EXPECT_THROW(program.deleteStartTime(5), invalid_id);

	Program::StartTimes::const_iterator it;

	// Delete from middle
	program.deleteStartTime(1);

	EXPECT_EQ(3, program.startTimes.size());

	it = program.startTimes.begin();
	EXPECT_EQ(0, it->first);
	EXPECT_EQ(10, it->second);

	it++;
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);

	it++;
	EXPECT_EQ(3, it->first);
	EXPECT_EQ(40, it->second);


	// Delete first
	program.deleteStartTime(0);

	EXPECT_EQ(2, program.startTimes.size());

	it = program.startTimes.begin();
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);

	it++;
	EXPECT_EQ(3, it->first);
	EXPECT_EQ(40, it->second);

	// Delete last
	program.deleteStartTime(3);

	EXPECT_EQ(1, program.startTimes.size());

	it = program.startTimes.begin();
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);
}

TEST(Program, GetStartTime) {
	Program program;

	program.addStartTime(10);
	program.addStartTime(20);
	program.addStartTime(30);

	EXPECT_EQ(10, program.getStartTime(0));
	EXPECT_EQ(20, program.getStartTime(1));
	EXPECT_EQ(30, program.getStartTime(2));
	EXPECT_THROW(program.getStartTime(3), invalid_id);
}

TEST(Program, SetStartTime) {
	Program program;

	program.addStartTime(10);
	program.addStartTime(20);

	EXPECT_NO_THROW(program.setStartTime(0, 100));
	EXPECT_NO_THROW(program.setStartTime(1, 200));
	EXPECT_THROW(program.setStartTime(2, 300), invalid_id);

	EXPECT_EQ(100, program.getStartTime(0));
	EXPECT_EQ(200, program.getStartTime(1));
}
