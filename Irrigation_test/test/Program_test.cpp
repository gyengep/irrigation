/*
 * Program_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include <gtest/gtest.h>

#include "exception.h"
#include "Program.h"


TEST(Program, name) {
	Program program;
	const char* name = "AbcXyz";

	EXPECT_EQ("", program.getName());

	program.setName(name);
	EXPECT_EQ(name, program.getName());
}
