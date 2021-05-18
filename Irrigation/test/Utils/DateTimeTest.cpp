#include <gmock/gmock.h>
#include "Utils/DateTime.h"

using namespace testing;


TEST(UtcDateTimeTest, init) {
	UtcDateTime dateTime(1621326808);

	EXPECT_THAT(dateTime.getYears(), Eq(2021));
	EXPECT_THAT(dateTime.getMonths(), Eq(5));
	EXPECT_THAT(dateTime.getDays(), Eq(18));
	EXPECT_THAT(dateTime.getHours(), Eq(8));
	EXPECT_THAT(dateTime.getMinutes(), Eq(33));
	EXPECT_THAT(dateTime.getSeconds(), Eq(28));
}

TEST(LocalDateTimeTest, init) {
	LocalDateTime dateTime(1621326808);

	EXPECT_THAT(dateTime.getYears(), Eq(2021));
	EXPECT_THAT(dateTime.getMonths(), Eq(5));
	EXPECT_THAT(dateTime.getDays(), Eq(18));
	EXPECT_THAT(dateTime.getHours(), Eq(10));
	EXPECT_THAT(dateTime.getMinutes(), Eq(33));
	EXPECT_THAT(dateTime.getSeconds(), Eq(28));
}

TEST(UtcDateTimeTest, toString) {
	UtcDateTime dateTime(1621326808);

	EXPECT_THAT(dateTime.toString("%Y-%m-%d %H:%M:%S"), Eq("2021-05-18 08:33:28"));
}

TEST(LocalDateTimeTest, toString) {
	LocalDateTime dateTime(1621326808);

	EXPECT_THAT(dateTime.toString("%Y-%m-%d %H:%M:%S"), Eq("2021-05-18 10:33:28"));
}

TEST(UtcDateTimeTest, create) {
	UtcDateTime dateTime = UtcDateTime::create(2021, 5, 18, 8, 33, 28);

	EXPECT_TRUE(dateTime == UtcDateTime(1621326808));
}

TEST(LocalDateTimeTest, create) {
	LocalDateTime dateTime = LocalDateTime::create(2021, 5, 18, 10, 33, 28);

	EXPECT_TRUE(dateTime == UtcDateTime(1621326808));
}

TEST(UtcDateTimeTest, createInvalid) {
	EXPECT_NO_THROW(UtcDateTime::create(2021, 5, 18, 10, 33, 28));

	EXPECT_THROW(UtcDateTime::create(2021, 0, 18, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 13, 18, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 2, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 2, 29, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 4, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 4, 31, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 32, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, -1, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, 24, 33, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, 8, -1, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, 8, 60, 28), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, 8, 33, -1), std::runtime_error);
	EXPECT_THROW(UtcDateTime::create(2021, 5, 18, 8, 33, 60), std::runtime_error);
}

TEST(LocalDateTimeTest, createInvalid) {
	EXPECT_NO_THROW(LocalDateTime::create(2021, 5, 18, 10, 33, 28));

	EXPECT_THROW(LocalDateTime::create(2021, 0, 18, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 13, 18, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 2, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 2, 29, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 4, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 4, 31, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 0, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 32, 8, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, -1, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, 24, 33, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, 8, -1, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, 8, 60, 28), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, 8, 33, -1), std::runtime_error);
	EXPECT_THROW(LocalDateTime::create(2021, 5, 18, 8, 33, 60), std::runtime_error);
}

TEST(UtcDateTimeTest, equal) {
	EXPECT_FALSE(UtcDateTime(1621326807) == UtcDateTime(1621326808));
	EXPECT_TRUE(UtcDateTime(1621326808) == UtcDateTime(1621326808));
	EXPECT_FALSE(UtcDateTime(1621326809) == UtcDateTime(1621326808));
}

TEST(LocalDateTimeTest, equal) {
	EXPECT_FALSE(LocalDateTime(1621326807) == LocalDateTime(1621326808));
	EXPECT_TRUE(LocalDateTime(1621326808) == LocalDateTime(1621326808));
	EXPECT_FALSE(LocalDateTime(1621326809) == LocalDateTime(1621326808));
}

TEST(UtcDateTimeTest, addDays) {
	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addDays(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(6));
		EXPECT_THAT(dateTime.getDays(), Eq(8));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addDays(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(4));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}
}

TEST(UtcDateTimeTest, addDaysDST) {
	{
		UtcDateTime dateTime = UtcDateTime(1616836821).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1616836821).addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(28));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(UtcDateTime(1616836821).addDays(1) == UtcDateTime(1616836821).addHours(24));
}

TEST(LocalDateTimeTest, addDays) {
	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addDays(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(6));
		EXPECT_THAT(dateTime.getDays(), Eq(8));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addDays(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(4));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addDaysDST1) {
	const std::time_t rawtime = 1616836821;

	{
		LocalDateTime dateTime = LocalDateTime(rawtime).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		LocalDateTime dateTime = LocalDateTime(rawtime).addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(28));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(LocalDateTime(rawtime).addDays(1) == LocalDateTime(rawtime).addHours(23));
}

TEST(LocalDateTimeTest, addDaysDST2) {
	const std::time_t rawtime = 1635582021;

	{
		LocalDateTime dateTime = LocalDateTime(rawtime).addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(10));
		EXPECT_THAT(dateTime.getDays(), Eq(30));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		LocalDateTime dateTime = LocalDateTime(rawtime).addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(10));
		EXPECT_THAT(dateTime.getDays(), Eq(31));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(LocalDateTime(rawtime).addDays(1) == LocalDateTime(rawtime).addHours(25));
}

TEST(UtcDateTimeTest, addHours) {
	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addHours(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addHours(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addHours(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(5));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addHours(72);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(21));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addHours(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(17));
		EXPECT_THAT(dateTime.getHours(), Eq(11));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addHours) {
	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addHours(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addHours(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(11));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addHours(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(7));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addHours(72);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(21));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addHours(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(17));
		EXPECT_THAT(dateTime.getHours(), Eq(13));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(UtcDateTimeTest, addMinutes) {
	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addMinutes(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addMinutes(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(34));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addMinutes(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(11));
		EXPECT_THAT(dateTime.getMinutes(), Eq(3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addMinutes(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(6));
		EXPECT_THAT(dateTime.getMinutes(), Eq(3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addMinutes(60 * 24);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addMinutes) {
	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addMinutes(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addMinutes(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(34));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addMinutes(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(13));
		EXPECT_THAT(dateTime.getMinutes(), Eq(3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addMinutes(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addMinutes(60 * 24);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(19));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}
}

TEST(UtcDateTimeTest, addSeconds) {
	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addSeconds(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addSeconds(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(29));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addSeconds(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(35));
		EXPECT_THAT(dateTime.getSeconds(), Eq(58));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addSeconds(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(58));
	}

	{
		UtcDateTime dateTime = UtcDateTime(1621326808).addSeconds(60 * 60);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addSeconds) {
	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addSeconds(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addSeconds(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(29));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addSeconds(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(35));
		EXPECT_THAT(dateTime.getSeconds(), Eq(58));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addSeconds(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(58));
	}

	{
		LocalDateTime dateTime = LocalDateTime(1621326808).addSeconds(60 * 60);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(11));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}
