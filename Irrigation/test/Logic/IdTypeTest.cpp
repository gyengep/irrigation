#include "Logic/IdType.h"
#include "Exceptions/Exceptions.h"
#include <gmock/gmock.h>

using namespace testing;


TEST(IdTypeTest, defaultConst) {
	IdType id1;
	IdType id2;

	EXPECT_THAT(id2.getValue(), Eq(id1.getValue() + 1));
}

TEST(IdTypeTest, initConst) {
	IdType id1;
	IdType id2(id1.getValue() + 50);
	IdType id3;

	EXPECT_THAT(id2.getValue(), Eq(id1.getValue() + 50));
	EXPECT_THAT(id3.getValue(), Eq(id2.getValue() + 1));
}

TEST(IdTypeTest, fromString) {
	EXPECT_THAT(IdType::from_string("1234"), Eq(1234));
	EXPECT_THAT(IdType::from_string("98765"), Eq(98765));
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
