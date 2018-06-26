#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/SchedulersDTO.h"

using namespace std;
using namespace testing;



TEST(SpecifiedSchedulerDTOTest, defaultConstructor) {
	SpecifiedSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
}

TEST(SpecifiedSchedulerDTOTest, constructor) {
	const list<bool> expectedValues({ false, true });
	SpecifiedSchedulerDTO schedulerDTO(new list<bool>(expectedValues));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(SpecifiedSchedulerDTOTest, copyConstructor) {
	const list<bool> expectedValues({ false, true, false, false });
	const SpecifiedSchedulerDTO other(new list<bool>(expectedValues));

	SpecifiedSchedulerDTO schedulerDTO(other);

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(SpecifiedSchedulerDTOTest, hasValues) {
	SpecifiedSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	schedulerDTO.setValues(new list<bool>());
	EXPECT_TRUE(schedulerDTO.hasValues());
}

TEST(SpecifiedSchedulerDTOTest, getValues) {
	const list<bool> expectedValues({ false, false, true });
	SpecifiedSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
	schedulerDTO.setValues(new list<bool>(expectedValues));
	ASSERT_NO_THROW(schedulerDTO.getValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(SpecifiedSchedulerDTOTest, equal) {
	SpecifiedSchedulerDTO schedulerDTO1;
	SpecifiedSchedulerDTO schedulerDTO2;

	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
	EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);

	const list<bool> expectedValues1({ false, true });
	const list<bool> expectedValues2({ false, true, true });

	schedulerDTO1.setValues(new list<bool>(expectedValues1));
	EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
	EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

	schedulerDTO2.setValues(new list<bool>(expectedValues2));
	EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);
	EXPECT_FALSE(schedulerDTO2 == schedulerDTO1);

	schedulerDTO1.setValues(new list<bool>(expectedValues2));
	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
	EXPECT_TRUE(schedulerDTO2 == schedulerDTO1);
}
