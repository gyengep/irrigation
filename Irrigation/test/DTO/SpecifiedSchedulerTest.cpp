#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/SchedulersDTO.h"

using namespace std;
using namespace testing;



TEST(WeeklySchedulerDTOTest, defaultConstructor) {
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasAdjustment());
	EXPECT_FALSE(schedulerDTO.hasValues());
}

TEST(WeeklySchedulerDTOTest, parametrizedConstructor) {
	const unsigned expectedAdjustment = 50;
	const list<bool> expectedValues({ false, true });
	WeeklySchedulerDTO schedulerDTO(expectedAdjustment, list<bool>(expectedValues));

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, copyConstructor) {
	const unsigned expectedAdjustment = 130;
	const list<bool> expectedValues({ false, true, false, false });
	const WeeklySchedulerDTO source(expectedAdjustment, list<bool>(expectedValues));

	WeeklySchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, moveConstructor) {
	const unsigned expectedAdjustment = 130;
	const list<bool> expectedValues({ false, true, false, false });
	WeeklySchedulerDTO source(expectedAdjustment, list<bool>(expectedValues));

	WeeklySchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasAdjustment());
	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, hasAdjustment) {
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasAdjustment());
	schedulerDTO.setAdjustment(100);
	EXPECT_TRUE(schedulerDTO.hasAdjustment());
}

TEST(WeeklySchedulerDTOTest, getAdjustment) {
	const unsigned expectedAdjustment = 130;
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getAdjustment(), logic_error);
	schedulerDTO.setAdjustment(expectedAdjustment);
	ASSERT_NO_THROW(schedulerDTO.getAdjustment());
	EXPECT_THAT(schedulerDTO.getAdjustment(), Eq(expectedAdjustment));
}

TEST(WeeklySchedulerDTOTest, hasValues) {
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	schedulerDTO.setValues(list<bool>());
	EXPECT_TRUE(schedulerDTO.hasValues());
}

TEST(WeeklySchedulerDTOTest, getValues) {
	const list<bool> expectedValues({ false, false, true });
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
	schedulerDTO.setValues(list<bool>(expectedValues));
	ASSERT_NO_THROW(schedulerDTO.getValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, equal) {
	WeeklySchedulerDTO schedulerDTO1;
	WeeklySchedulerDTO schedulerDTO2;

	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
	EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);

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
