#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/Parser.h"

using namespace std;


TEST(Parser, parseUnsigned) {
	EXPECT_EQ(2465, Parser::parseUnsigned("2465"));
}

TEST(Parser, parseUnsignedInvalid) {
	EXPECT_THROW(Parser::parseUnsigned("abc"), invalid_argument);
	EXPECT_THROW(Parser::parseUnsigned("123 "), invalid_argument);
	EXPECT_THROW(Parser::parseUnsigned("1234a"), invalid_argument);
	EXPECT_THROW(Parser::parseUnsigned("a1234"), invalid_argument);
	EXPECT_THROW(Parser::parseUnsigned("12 34"), invalid_argument);
}

TEST(Parser, fromStringOutOfRange) {
	EXPECT_NO_THROW(Parser::parseUnsigned("4294967295"));
	EXPECT_THROW(Parser::parseUnsigned("4294967296"), out_of_range);
	EXPECT_NO_THROW(Parser::parseUnsigned("4294967295"));
	EXPECT_THROW(Parser::parseUnsigned("4294967296"), out_of_range);
}

