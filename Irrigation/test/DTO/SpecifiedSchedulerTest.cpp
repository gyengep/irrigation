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
	SpecifiedSchedulerDTO schedulerDTO(new list<bool>({ false, true }));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(list<bool>({ false, true })));
}

TEST(SpecifiedSchedulerDTOTest, copyConstructor) {
	SpecifiedSchedulerDTO schedulerDTO(SpecifiedSchedulerDTO(new list<bool>({ false, true })));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(list<bool>({ false, true })));
}

TEST(SpecifiedSchedulerDTOTest, getValues) {
	SpecifiedSchedulerDTO schedulerDTO;

	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
	schedulerDTO.setValues(new list<bool>({ true, false, true }));
	ASSERT_NO_THROW(schedulerDTO.getValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(list<bool>({ true, false, true })));
}

TEST(SpecifiedSchedulerDTOTest, hasValues) {
	SpecifiedSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	schedulerDTO.setValues(new list<bool>({ false, false, true }));
	EXPECT_TRUE(schedulerDTO.hasValues());
}

TEST(SpecifiedSchedulerDTOTest, equal) {
	SpecifiedSchedulerDTO schedulerDTO1;
	SpecifiedSchedulerDTO schedulerDTO2;

	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);

	schedulerDTO1.setValues(new list<bool>({ false, true }));
	EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);

	schedulerDTO2.setValues(new list<bool>({ false, true, true }));
	EXPECT_FALSE(schedulerDTO1 == schedulerDTO2);

	schedulerDTO1.setValues(new list<bool>({ false, true, true }));
	EXPECT_TRUE(schedulerDTO1 == schedulerDTO2);
}
