#include "Logic/IdType.h"
#include "Exceptions/Exceptions.h"
#include <gmock/gmock.h>

using namespace testing;


TEST(IdTypeTest, defaultConst) {
	IdType id1;
	IdType id2;

	const unsigned value1 = id1;
	const unsigned value2 = id2;

	EXPECT_THAT(value2, Eq(value1 + 1));
}

TEST(IdTypeTest, initConst) {
	const unsigned refValue = IdType();

	IdType id1(refValue + 50);
	IdType id2;

	const unsigned value1 = id1;
	const unsigned value2 = id2;

	EXPECT_THAT(value1, Eq(refValue + 50));
	EXPECT_THAT(value2, Eq(value1 + 1));
}

TEST(IdTypeTest, initLess) {
	const unsigned refValue = IdType();

	IdType id1(refValue + 100);
	IdType id2(refValue + 50);
	IdType id3;

	const unsigned value1 = id1;
	const unsigned value2 = id2;
	const unsigned value3 = id3;

	EXPECT_THAT(value1, Eq(refValue + 100));
	EXPECT_THAT(value2, Eq(refValue + 50));
	EXPECT_THAT(value3, Eq(refValue + 101));
}

TEST(IdTypeTest, fromString) {
	EXPECT_THAT(IdType::from_string("1234"), Eq(1234));
	EXPECT_THAT(IdType::from_string("4294967295"), Eq(4294967295));
}

TEST(IdTypeTest, fromStringInvalid) {
	EXPECT_THROW(IdType::from_string(""), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string(" "), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string("a"), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string("1a"), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string("a1"), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string("1 "), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string(" 1"), IllegalArgumentException);
	EXPECT_THROW(IdType::from_string("1 2"), IllegalArgumentException);
}
