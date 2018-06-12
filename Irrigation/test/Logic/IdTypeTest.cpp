#include <gmock/gmock.h>
#include "Logic/IdType.h"



TEST(IdTypeTest, defaultConst) {
	IdType id1;
	IdType id2;

	const unsigned value1 = id1;
	const unsigned value2 = id2;

	EXPECT_EQ(value1 + 1 , value2);
}

TEST(IdTypeTest, initConst) {
	const unsigned refValue = IdType();

	IdType id1(refValue + 50);
	IdType id2;

	const unsigned value1 = id1;
	const unsigned value2 = id2;

	EXPECT_EQ(refValue + 50, value1);
	EXPECT_EQ(value1 + 1 , value2);
}

TEST(IdTypeTest, initLess) {
	const unsigned refValue = IdType();

	IdType id1(refValue + 100);
	IdType id2(refValue + 50);
	IdType id3;

	const unsigned value1 = id1;
	const unsigned value2 = id2;
	const unsigned value3 = id3;

	EXPECT_EQ(refValue + 100, value1);
	EXPECT_EQ(refValue + 50, value2);
	EXPECT_EQ(refValue + 101, value3);
}
