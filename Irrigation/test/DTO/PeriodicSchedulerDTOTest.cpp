#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/PeriodicSchedulerDTO.h"

using namespace std;
using namespace testing;



TEST(PeriodicSchedulerDTOTest, defaultConstructor) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasAdjustment());
	EXPECT_FALSE(schedulerDTO.hasValues());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartYear());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartDay());
}

TEST(PeriodicSchedulerDTOTest, parametrizedConstructor) {
	const unsigned expectedAdjustment = 50;
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;
	PeriodicSchedulerDTO schedulerDTO(expectedAdjustment, list<bool>(expectedValues),
			expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, copyConstructor) {
	const unsigned expectedAdjustment = 50;
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;
	const PeriodicSchedulerDTO source(expectedAdjustment, list<bool>(expectedValues),
			expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);

	PeriodicSchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, moveConstructor) {
	const unsigned expectedAdjustment = 50;
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;
	PeriodicSchedulerDTO source(expectedAdjustment, list<bool>(expectedValues),
			expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);

	PeriodicSchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, hasAdjustment) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasAdjustment());
	schedulerDTO.setAdjustment(100);
	EXPECT_TRUE(schedulerDTO.hasAdjustment());
}

TEST(PeriodicSchedulerDTOTest, getAdjustment) {
	const unsigned expectedAdjustment = 130;
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getAdjustment(), logic_error);
	schedulerDTO.setAdjustment(expectedAdjustment);
	ASSERT_NO_THROW(schedulerDTO.getAdjustment());
	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
}

TEST(PeriodicSchedulerDTOTest, hasValues) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	schedulerDTO.setValues(list<bool>());
	EXPECT_TRUE(schedulerDTO.hasValues());
}

TEST(PeriodicSchedulerDTOTest, getValues) {
	const list<bool> expectedValues({ false, false, true });
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
	schedulerDTO.setValues(list<bool>(expectedValues));
	ASSERT_NO_THROW(schedulerDTO.getValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(PeriodicSchedulerDTOTest, hasPeriodStartYear) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasPeriodStartYear());
	schedulerDTO.setPeriodStartYear(6);
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
}

TEST(PeriodicSchedulerDTOTest, getPeriodStartYear) {
	const unsigned expectedPeriodStartYear = 100;
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getPeriodStartYear(), logic_error);
	schedulerDTO.setPeriodStartYear(expectedPeriodStartYear);
	ASSERT_NO_THROW(schedulerDTO.getPeriodStartYear());
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
}

TEST(PeriodicSchedulerDTOTest, hasPeriodStartMonth) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasPeriodStartMonth());
	schedulerDTO.setPeriodStartMonth(10);
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
}

TEST(PeriodicSchedulerDTOTest, getPeriodStartMonth) {
	const unsigned expectedPeriodStartMonth = 11;
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getPeriodStartMonth(), logic_error);
	schedulerDTO.setPeriodStartMonth(expectedPeriodStartMonth);
	ASSERT_NO_THROW(schedulerDTO.getPeriodStartMonth());
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMonth));
}

TEST(PeriodicSchedulerDTOTest, hasPeriodStartDay) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasPeriodStartDay());
	schedulerDTO.setPeriodStartDay(25);
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());
}

TEST(PeriodicSchedulerDTOTest, getPeriodStartDay) {
	const unsigned expectedPeriodStartDay = 21;
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getPeriodStartDay(), logic_error);
	schedulerDTO.setPeriodStartDay(expectedPeriodStartDay);
	ASSERT_NO_THROW(schedulerDTO.getPeriodStartDay());
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, equalsOperator) {
	PeriodicSchedulerDTO schedulerDTO1;
	PeriodicSchedulerDTO schedulerDTO2;

	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
	EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);

	{
		const list<bool> expectedValues1({ false, true });
		const list<bool> expectedValues2({ false, true, true });

		schedulerDTO1.setValues(list<bool>(expectedValues1));
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO2.setValues(list<bool>(expectedValues2));
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO1.setValues(list<bool>(expectedValues2));
		EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
		EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
	}

	{
		const unsigned expectedAdjustment1 = 40;
		const unsigned expectedAdjustment2 = 60;

		schedulerDTO1.setAdjustment(expectedAdjustment1);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO2.setAdjustment(expectedAdjustment2);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO1.setAdjustment(expectedAdjustment2);
		EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
		EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
	}

	{
		const unsigned expectedPeriodStartYear1 = 1900;
		const unsigned expectedPeriodStartYear2 = 2000;

		schedulerDTO1.setPeriodStartYear(expectedPeriodStartYear1);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO2.setPeriodStartYear(expectedPeriodStartYear2);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO1.setPeriodStartYear(expectedPeriodStartYear2);
		EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
		EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
	}

	{
		const unsigned expectedPeriodStartMonth1 = 1900;
		const unsigned expectedPeriodStartMonth2 = 2000;

		schedulerDTO1.setPeriodStartMonth(expectedPeriodStartMonth1);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO2.setPeriodStartMonth(expectedPeriodStartMonth2);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO1.setPeriodStartMonth(expectedPeriodStartMonth2);
		EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
		EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
	}

	{
		const unsigned expectedPeriodStartDay1 = 1900;
		const unsigned expectedPeriodStartDay2 = 2000;

		schedulerDTO1.setPeriodStartDay(expectedPeriodStartDay1);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO2.setPeriodStartDay(expectedPeriodStartDay2);
		EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
		EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

		schedulerDTO1.setPeriodStartDay(expectedPeriodStartDay2);
		EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
		EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
	}
}
