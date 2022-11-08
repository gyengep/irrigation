#include <gmock/gmock.h>
#include "Utils/DateTime.h"

using namespace testing;

const int expectedYear   = 2021;
const int expectedMonth  = 5;
const int expectedDay    = 18;
const int expectedHour   = 8;
const int expectedMinute = 33;
const int expectedSecond = 28;


TEST(DateTimeTest, init) {
	DateTime now1 = DateTime::now();
	const DateTime now2(DateTime::now());
	const DateTime now3(now2);
}

TEST(UtcDateTimeTest, now) {
	UtcDateTime::now();
}

TEST(UtcDateTimeTest, init) {
	UtcDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_THAT(dateTime.getYears(), Eq(expectedYear));
	EXPECT_THAT(dateTime.getMonths(), Eq(expectedMonth));
	EXPECT_THAT(dateTime.getDays(), Eq(expectedDay));
	EXPECT_THAT(dateTime.getHours(), Eq(expectedHour));
	EXPECT_THAT(dateTime.getMinutes(), Eq(expectedMinute));
	EXPECT_THAT(dateTime.getSeconds(), Eq(expectedSecond));
}

TEST(LocalDateTimeTest, now) {
	LocalDateTime::now();
}

TEST(LocalDateTimeTest, init) {
	LocalDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_THAT(dateTime.getYears(), Eq(expectedYear));
	EXPECT_THAT(dateTime.getMonths(), Eq(expectedMonth));
	EXPECT_THAT(dateTime.getDays(), Eq(expectedDay));
	EXPECT_THAT(dateTime.getHours(), Eq(expectedHour));
	EXPECT_THAT(dateTime.getMinutes(), Eq(expectedMinute));
	EXPECT_THAT(dateTime.getSeconds(), Eq(expectedSecond));
}

TEST(UtcDateTimeTest, dayOfWeek) {
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28).getDayOfWeek(), Eq(6)); // SUN
	EXPECT_THAT(UtcDateTime(2021, 5, 17, 8, 33, 28).getDayOfWeek(), Eq(0)); // MON
	EXPECT_THAT(UtcDateTime(2021, 5, 18, 8, 33, 28).getDayOfWeek(), Eq(1)); // TUE
	EXPECT_THAT(UtcDateTime(2021, 5, 19, 8, 33, 28).getDayOfWeek(), Eq(2)); // WED
	EXPECT_THAT(UtcDateTime(2021, 5, 20, 8, 33, 28).getDayOfWeek(), Eq(3)); // THU
	EXPECT_THAT(UtcDateTime(2021, 5, 21, 8, 33, 28).getDayOfWeek(), Eq(4)); // FRI
	EXPECT_THAT(UtcDateTime(2021, 5, 22, 8, 33, 28).getDayOfWeek(), Eq(5)); // SAT
	EXPECT_THAT(UtcDateTime(2021, 5, 23, 8, 33, 28).getDayOfWeek(), Eq(6)); // SUN
	EXPECT_THAT(UtcDateTime(2021, 5, 24, 8, 33, 28).getDayOfWeek(), Eq(0)); // MON
}

TEST(LocalDateTimeTest, dayOfWeek) {
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28).getDayOfWeek(), Eq(6)); // SUN
	EXPECT_THAT(LocalDateTime(2021, 5, 17, 8, 33, 28).getDayOfWeek(), Eq(0)); // MON
	EXPECT_THAT(LocalDateTime(2021, 5, 18, 8, 33, 28).getDayOfWeek(), Eq(1)); // TUE
	EXPECT_THAT(LocalDateTime(2021, 5, 19, 8, 33, 28).getDayOfWeek(), Eq(2)); // WED
	EXPECT_THAT(LocalDateTime(2021, 5, 20, 8, 33, 28).getDayOfWeek(), Eq(3)); // THU
	EXPECT_THAT(LocalDateTime(2021, 5, 21, 8, 33, 28).getDayOfWeek(), Eq(4)); // FRI
	EXPECT_THAT(LocalDateTime(2021, 5, 22, 8, 33, 28).getDayOfWeek(), Eq(5)); // SAT
	EXPECT_THAT(LocalDateTime(2021, 5, 23, 8, 33, 28).getDayOfWeek(), Eq(6)); // SUN
	EXPECT_THAT(LocalDateTime(2021, 5, 24, 8, 33, 28).getDayOfWeek(), Eq(0)); // MON
}

TEST(UtcDateTimeTest, diff) {
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28) -     UtcDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28 + 5) - UtcDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::seconds(5)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28) -     UtcDateTime(2021, 5, 16, 8, 33, 28 + 5), Eq(std::chrono::seconds(-5)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33 + 3, 28) - UtcDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::minutes(3)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28) -     UtcDateTime(2021, 5, 16, 8, 33 + 3, 28), Eq(std::chrono::minutes(-3)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8 + 7, 33, 28) - UtcDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::hours(7)));
	EXPECT_THAT(UtcDateTime(2021, 5, 16, 8, 33, 28) -     UtcDateTime(2021, 5, 16, 8 + 7, 33, 28), Eq(std::chrono::hours(-7)));
}

TEST(LocalDateTimeTest, diff) {
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28) -     LocalDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28 + 5) - LocalDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::seconds(5)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28) -     LocalDateTime(2021, 5, 16, 8, 33, 28 + 5), Eq(std::chrono::seconds(-5)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33 + 3, 28) - LocalDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::minutes(3)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28) -     LocalDateTime(2021, 5, 16, 8, 33 + 3, 28), Eq(std::chrono::minutes(-3)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8 + 7, 33, 28) - LocalDateTime(2021, 5, 16, 8, 33, 28), Eq(std::chrono::hours(7)));
	EXPECT_THAT(LocalDateTime(2021, 5, 16, 8, 33, 28) -     LocalDateTime(2021, 5, 16, 8 + 7, 33, 28), Eq(std::chrono::hours(-7)));
}

TEST(UtcDateTimeTest, operatorPlus) {
	const UtcDateTime dateTime(2021, 5, 16, 8, 33, 28);

	EXPECT_THAT(dateTime + std::chrono::seconds(5), Eq(UtcDateTime(2021, 5, 16, 8, 33, 28 + 5)));
	EXPECT_THAT(dateTime - std::chrono::seconds(5), Eq(UtcDateTime(2021, 5, 16, 8, 33, 28 - 5)));
	EXPECT_THAT(dateTime + std::chrono::minutes(6), Eq(UtcDateTime(2021, 5, 16, 8, 33 + 6, 28)));
	EXPECT_THAT(dateTime - std::chrono::minutes(6), Eq(UtcDateTime(2021, 5, 16, 8, 33 - 6, 28)));
	EXPECT_THAT(dateTime + std::chrono::hours(7),   Eq(UtcDateTime(2021, 5, 16, 8 + 7, 33, 28)));
	EXPECT_THAT(dateTime - std::chrono::hours(7),   Eq(UtcDateTime(2021, 5, 16, 8 - 7, 33, 28)));
}

TEST(LocalDateTimeTest, operatorPlus) {
	const LocalDateTime dateTime(2021, 5, 16, 8, 33, 28);

	EXPECT_THAT(dateTime + std::chrono::seconds(5), Eq(LocalDateTime(2021, 5, 16, 8, 33, 28 + 5)));
	EXPECT_THAT(dateTime - std::chrono::seconds(5), Eq(LocalDateTime(2021, 5, 16, 8, 33, 28 - 5)));
	EXPECT_THAT(dateTime + std::chrono::minutes(6), Eq(LocalDateTime(2021, 5, 16, 8, 33 + 6, 28)));
	EXPECT_THAT(dateTime - std::chrono::minutes(6), Eq(LocalDateTime(2021, 5, 16, 8, 33 - 6, 28)));
	EXPECT_THAT(dateTime + std::chrono::hours(7),   Eq(LocalDateTime(2021, 5, 16, 8 + 7, 33, 28)));
	EXPECT_THAT(dateTime - std::chrono::hours(7),   Eq(LocalDateTime(2021, 5, 16, 8 - 7, 33, 28)));
}

TEST(UtcDateTimeTest, toString) {
	UtcDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_THAT(dateTime.toString("%Y-%m-%d %H:%M:%S"), Eq("2021-05-18 08:33:28"));
}

TEST(LocalDateTimeTest, toString) {
	LocalDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_THAT(dateTime.toString("%Y-%m-%d %H:%M:%S"), Eq("2021-05-18 08:33:28"));
}

TEST(UtcDateTimeTest, initInvalid) {
	EXPECT_NO_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond));

	EXPECT_THROW(UtcDateTime(expectedYear, 0,  expectedDay, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 13, expectedDay, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 2, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 2, 29, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 4, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 4, 31, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 5, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, 5, 32, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, -1, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, 24, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, -1, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, 60, expectedSecond), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, -1), std::runtime_error);
	EXPECT_THROW(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, 60), std::runtime_error);
}

TEST(LocalDateTimeTest, createInvalid) {
	EXPECT_NO_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond));

	EXPECT_THROW(LocalDateTime(expectedYear, 0,  expectedDay, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 13, expectedDay, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 2, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 2, 29, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 4, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 4, 31, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 5, 0,  expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, 5, 32, expectedHour, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, -1, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, 24, expectedMinute, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, -1, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, 60, expectedSecond), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, -1), std::runtime_error);
	EXPECT_THROW(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, 60), std::runtime_error);
}

TEST(UtcDateTimeTest, isEqual) {
	UtcDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_FALSE(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond - 1) == dateTime);
	EXPECT_TRUE(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond) == dateTime);
	EXPECT_FALSE(UtcDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond + 1) == dateTime);
}

TEST(LocalDateTimeTest, isEqual) {
	LocalDateTime dateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);

	EXPECT_FALSE(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond - 1) == dateTime);
	EXPECT_TRUE(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond) == dateTime);
	EXPECT_FALSE(LocalDateTime(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond + 1) == dateTime);
}

TEST(UtcDateTimeTest, equalsOperator) {
	const UtcDateTime dateTimeToCopy(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);
	UtcDateTime dateTime(2019, 1, 1, 0, 0, 0);

	EXPECT_THAT(dateTime.getYears(), Not(Eq(expectedYear)));
	EXPECT_THAT(dateTime.getMonths(), Not(Eq(expectedMonth)));
	EXPECT_THAT(dateTime.getDays(), Not(Eq(expectedDay)));
	EXPECT_THAT(dateTime.getHours(), Not(Eq(expectedHour)));
	EXPECT_THAT(dateTime.getMinutes(), Not(Eq(expectedMinute)));
	EXPECT_THAT(dateTime.getSeconds(), Not(Eq(expectedSecond)));

	dateTime = dateTimeToCopy;

	EXPECT_THAT(dateTime.getYears(), Eq(expectedYear));
	EXPECT_THAT(dateTime.getMonths(), Eq(expectedMonth));
	EXPECT_THAT(dateTime.getDays(), Eq(expectedDay));
	EXPECT_THAT(dateTime.getHours(), Eq(expectedHour));
	EXPECT_THAT(dateTime.getMinutes(), Eq(expectedMinute));
	EXPECT_THAT(dateTime.getSeconds(), Eq(expectedSecond));
}

TEST(LocalDateTimeTest, equalsOperator) {
	const LocalDateTime dateTimeToCopy(expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond);
	LocalDateTime dateTime(2019, 1, 1, 0, 0, 0);

	EXPECT_THAT(dateTime.getYears(), Not(Eq(expectedYear)));
	EXPECT_THAT(dateTime.getMonths(), Not(Eq(expectedMonth)));
	EXPECT_THAT(dateTime.getDays(), Not(Eq(expectedDay)));
	EXPECT_THAT(dateTime.getHours(), Not(Eq(expectedHour)));
	EXPECT_THAT(dateTime.getMinutes(), Not(Eq(expectedMinute)));
	EXPECT_THAT(dateTime.getSeconds(), Not(Eq(expectedSecond)));

	dateTime = dateTimeToCopy;

	EXPECT_THAT(dateTime.getYears(), Eq(expectedYear));
	EXPECT_THAT(dateTime.getMonths(), Eq(expectedMonth));
	EXPECT_THAT(dateTime.getDays(), Eq(expectedDay));
	EXPECT_THAT(dateTime.getHours(), Eq(expectedHour));
	EXPECT_THAT(dateTime.getMinutes(), Eq(expectedMinute));
	EXPECT_THAT(dateTime.getSeconds(), Eq(expectedSecond));
}

TEST(UtcDateTimeTest, addDays) {
	const UtcDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5 + 1));
		EXPECT_THAT(dateTime.getDays(), Eq(18 - 10));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5 - 1));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 9));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(UtcDateTimeTest, addDaysDST) {
	const UtcDateTime dateTimeDefault(2021, 3, 27, 9, 20, 21);

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(9));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(dateTimeDefault.addDays(1) == dateTimeDefault.addHours(24));
}

TEST(LocalDateTimeTest, addDays) {
	const LocalDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5 + 1));
		EXPECT_THAT(dateTime.getDays(), Eq(18 - 10));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5 - 1));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 9));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addDaysDST1) {
	const LocalDateTime dateTimeDefault(2021, 3, 27, 10, 20, 21);

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(3));
		EXPECT_THAT(dateTime.getDays(), Eq(27 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(dateTimeDefault.addDays(1) == dateTimeDefault.addHours(23));
}

TEST(LocalDateTimeTest, addDaysDST2) {
	const LocalDateTime dateTimeDefault(2021, 10, 30, 10, 20, 21);

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(10));
		EXPECT_THAT(dateTime.getDays(), Eq(30));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addDays(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(10));
		EXPECT_THAT(dateTime.getDays(), Eq(30 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(10));
		EXPECT_THAT(dateTime.getMinutes(), Eq(20));
		EXPECT_THAT(dateTime.getSeconds(), Eq(21));
	}

	EXPECT_TRUE(dateTimeDefault.addDays(1) == dateTimeDefault.addHours(25));
}

TEST(UtcDateTimeTest, addHours) {
	const UtcDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		UtcDateTime dateTime = dateTimeDefault.addHours(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addHours(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 1));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addHours(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq((8 + 21) % 24));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addHours(72);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 3));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addHours(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(17));
		EXPECT_THAT(dateTime.getHours(), Eq(11));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addHours) {
	const LocalDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		LocalDateTime dateTime = dateTimeDefault.addHours(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = dateTimeDefault.addHours(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 1));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addHours(21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8 - 3));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addHours(72);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 3));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addHours(-21);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 - 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 3));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(UtcDateTimeTest, addMinutes) {
	const UtcDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		UtcDateTime dateTime = dateTimeDefault.addMinutes(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addMinutes(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 + 1));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addMinutes(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 3));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addMinutes(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 - 2));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addMinutes(60 * 24);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addMinutes) {
	const LocalDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		LocalDateTime dateTime = dateTimeDefault.addMinutes(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = dateTimeDefault.addMinutes(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 + 1));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addMinutes(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 3));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}

	{
		LocalDateTime dateTime = dateTimeDefault.addMinutes(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 - 2));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 30));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addMinutes(60 * 24);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18 + 1));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));

	}
}

TEST(UtcDateTimeTest, addSeconds) {
	const UtcDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		UtcDateTime dateTime = dateTimeDefault.addSeconds(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addSeconds(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 1));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addSeconds(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 + 2));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 30));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addSeconds(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 30));
	}

	{
		UtcDateTime dateTime = dateTimeDefault.addSeconds(60 * 60);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 1));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}

TEST(LocalDateTimeTest, addSeconds) {
	const LocalDateTime dateTimeDefault(2021, 5, 18, 8, 33, 28);

	{
		LocalDateTime dateTime = dateTimeDefault.addSeconds(0);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addSeconds(1);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 1));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addSeconds(150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 + 2));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 30));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addSeconds(-150);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33 - 3));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28 + 30));
	}

	{
		LocalDateTime dateTime = dateTimeDefault.addSeconds(60 * 60);

		EXPECT_THAT(dateTime.getYears(), Eq(2021));
		EXPECT_THAT(dateTime.getMonths(), Eq(5));
		EXPECT_THAT(dateTime.getDays(), Eq(18));
		EXPECT_THAT(dateTime.getHours(), Eq(8 + 1));
		EXPECT_THAT(dateTime.getMinutes(), Eq(33));
		EXPECT_THAT(dateTime.getSeconds(), Eq(28));
	}
}
