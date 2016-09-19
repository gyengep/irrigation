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

