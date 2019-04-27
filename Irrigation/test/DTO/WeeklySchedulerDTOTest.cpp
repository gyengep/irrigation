#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/WeeklySchedulerDTO.h"

using namespace std;
using namespace testing;



TEST(WeeklySchedulerDTOTest, defaultConstructor) {
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
}

TEST(WeeklySchedulerDTOTest, parametrizedConstructor) {
	const list<bool> expectedValues({ false, true });
	WeeklySchedulerDTO schedulerDTO(move(list<bool>(expectedValues)));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, copyConstructor) {
	const list<bool> expectedValues({ false, true, false, false });
	const WeeklySchedulerDTO source(move(list<bool>(expectedValues)));

	WeeklySchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, moveConstructor) {
	const list<bool> expectedValues({ false, true, false, false });
	WeeklySchedulerDTO source(move(list<bool>(expectedValues)));

	WeeklySchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
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

TEST(WeeklySchedulerDTOTest, equalsOperator) {
	WeeklySchedulerDTO schedulerDTO1;
	WeeklySchedulerDTO schedulerDTO2;

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
}
