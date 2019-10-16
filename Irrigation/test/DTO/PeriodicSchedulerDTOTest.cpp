#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/PeriodicSchedulerDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(PeriodicSchedulerDTOTest, defaultConstructor) {
	PeriodicSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartYear());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_FALSE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
	EXPECT_THROW(schedulerDTO.getPeriodStartYear(), logic_error);
	EXPECT_THROW(schedulerDTO.getPeriodStartMonth(), logic_error);
	EXPECT_THROW(schedulerDTO.getPeriodStartDay(), logic_error);
}

TEST(PeriodicSchedulerDTOTest, parametrizedConstructor) {
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;
	PeriodicSchedulerDTO schedulerDTO(list<bool>(expectedValues),
			expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, copyConstructor) {
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;

	const PeriodicSchedulerDTO source(list<bool>(expectedValues), expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);
	const PeriodicSchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerDTOTest, moveConstructor) {
	const list<bool> expectedValues({ false, true, false, true, false });
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMon = 5;
	const unsigned expectedPeriodStartDay = 15;

	PeriodicSchedulerDTO source(list<bool>(expectedValues), expectedPeriodStartYear, expectedPeriodStartMon, expectedPeriodStartDay);
	const PeriodicSchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartYear());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartMonth());
	EXPECT_TRUE(schedulerDTO.hasPeriodStartDay());

	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
	EXPECT_THAT(schedulerDTO.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(schedulerDTO.getPeriodStartMonth(), Eq(expectedPeriodStartMon));
	EXPECT_THAT(schedulerDTO.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(PeriodicSchedulerDTO, list<bool>, Values, list<bool>({ false, false, true }));
CHECK_DTO_FUNCTIONS(PeriodicSchedulerDTO, unsigned, PeriodStartYear, 6);
CHECK_DTO_FUNCTIONS(PeriodicSchedulerDTO, unsigned, PeriodStartMonth, 10);
CHECK_DTO_FUNCTIONS(PeriodicSchedulerDTO, unsigned, PeriodStartDay, 21);

///////////////////////////////////////////////////////////////////////////////

TEST(PeriodicSchedulerDTOTest, equalsOperator) {
	PeriodicSchedulerDTO dto1;
	PeriodicSchedulerDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(list<bool>, Values,
			list<bool>({ false, true }),
			list<bool>({ false, true, true })
		);
	CHECK_DTO_EQUALS_COPY(unsigned, PeriodStartYear, 1900, 200);
	CHECK_DTO_EQUALS_COPY(unsigned, PeriodStartMonth, 10, 11);
	CHECK_DTO_EQUALS_COPY(unsigned, PeriodStartDay, 5, 20);
}
