//============================================================================
// Name        : Irrigation_test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h"


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	//::testing::GTEST_FLAG(filter) = "*Container";
	return RUN_ALL_TESTS();
}
