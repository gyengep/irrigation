#include "common.h"
#include <gmock/gmock.h>
#include "Utils/Parse.h"


TEST(Command, parseProgramId) {
	EXPECT_EQ(0, Parser::parseProgramId("0"));
	EXPECT_EQ(10, Parser::parseProgramId("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseProgramId("4294967295"));
}

TEST(Command, parseProgramIdFailed) {
	EXPECT_THROW(Parser::parseProgramId("-1"), InvalidProgramIdException);
	EXPECT_THROW(Parser::parseProgramId("4294967296"), InvalidProgramIdException);
	EXPECT_THROW(Parser::parseProgramId("4294 67296"), InvalidProgramIdException);
	EXPECT_THROW(Parser::parseProgramId("1B2"), InvalidProgramIdException);
	EXPECT_THROW(Parser::parseProgramId("abc"), InvalidProgramIdException);
}

TEST(Command, parseStartTimeId) {
	EXPECT_EQ(0, Parser::parseStartTimeId("0"));
	EXPECT_EQ(10, Parser::parseStartTimeId("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseStartTimeId("4294967295"));
}

TEST(Command, parseStartTimeIdFailed) {
	EXPECT_THROW(Parser::parseStartTimeId("-1"), InvalidStartTimeIdException);
	EXPECT_THROW(Parser::parseStartTimeId("4294967296"), InvalidStartTimeIdException);
	EXPECT_THROW(Parser::parseStartTimeId("4294 67296"), InvalidStartTimeIdException);
	EXPECT_THROW(Parser::parseStartTimeId("1B2"), InvalidStartTimeIdException);
	EXPECT_THROW(Parser::parseStartTimeId("abc"), InvalidStartTimeIdException);
}

TEST(Command, parseRunTimeId) {
	EXPECT_EQ(0, Parser::parseRunTimeId("0"));
	EXPECT_EQ(10, Parser::parseRunTimeId("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseRunTimeId("4294967295"));
}

TEST(Command, parseRunTimeIdFailed) {
	EXPECT_THROW(Parser::parseRunTimeId("-1"), InvalidRunTimeIdException);
	EXPECT_THROW(Parser::parseRunTimeId("4294967296"), InvalidRunTimeIdException);
	EXPECT_THROW(Parser::parseRunTimeId("4294 67296"), InvalidRunTimeIdException);
	EXPECT_THROW(Parser::parseRunTimeId("1B2"), InvalidRunTimeIdException);
	EXPECT_THROW(Parser::parseRunTimeId("abc"), InvalidRunTimeIdException);
}

TEST(Command, parseUnsigned) {
	EXPECT_EQ(0, Parser::parseUnsigned("0"));
	EXPECT_EQ(10, Parser::parseUnsigned("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseUnsigned("4294967295"));
}

TEST(Command, parseUnsignedFailed) {
	EXPECT_THROW(Parser::parseUnsigned("-1"), InvalidParameterException);
	EXPECT_THROW(Parser::parseUnsigned("4294967296"), InvalidParameterException);
	EXPECT_THROW(Parser::parseUnsigned("4294 67296"), InvalidParameterException);
	EXPECT_THROW(Parser::parseUnsigned("1B2"), InvalidParameterException);
	EXPECT_THROW(Parser::parseUnsigned("abc"), InvalidParameterException);
}

TEST(Command, parseZoneId) {
	EXPECT_EQ(0, Parser::parseZoneId("0"));
	EXPECT_EQ(10, Parser::parseZoneId("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseZoneId("4294967295"));
}

TEST(Command, parseZoneIdFailed) {
	EXPECT_THROW(Parser::parseZoneId("-1"), InvalidZoneIdException);
	EXPECT_THROW(Parser::parseZoneId("4294967296"), InvalidZoneIdException);
	EXPECT_THROW(Parser::parseZoneId("4294 67296"), InvalidZoneIdException);
	EXPECT_THROW(Parser::parseZoneId("1B2"), InvalidZoneIdException);
	EXPECT_THROW(Parser::parseZoneId("abc"), InvalidZoneIdException);
}

TEST(Command, parseValveId) {
	EXPECT_EQ(0, Parser::parseValveId("0"));
	EXPECT_EQ(10, Parser::parseValveId("10"));
	EXPECT_EQ(0xFFFFFFFF, Parser::parseValveId("4294967295"));
}

TEST(Command, parseValveIdFailed) {
	EXPECT_THROW(Parser::parseValveId("-1"), InvalidValveIdException);
	EXPECT_THROW(Parser::parseValveId("4294967296"), InvalidValveIdException);
	EXPECT_THROW(Parser::parseValveId("4294 67296"), InvalidValveIdException);
	EXPECT_THROW(Parser::parseValveId("1B2"), InvalidValveIdException);
	EXPECT_THROW(Parser::parseValveId("abc"), InvalidValveIdException);
}

TEST(Command, parseOnOff) {
	EXPECT_EQ(true, Parser::parseOnOff("on"));
	EXPECT_EQ(false, Parser::parseOnOff("off"));
}

TEST(Command, parseOnOffFailed) {
	EXPECT_THROW(Parser::parseOnOff("on "), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff(" on"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("on1"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("-1"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("4294967296"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("4294 67296"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("1B2"), InvalidParameterException);
	EXPECT_THROW(Parser::parseOnOff("abc"), InvalidParameterException);
}
