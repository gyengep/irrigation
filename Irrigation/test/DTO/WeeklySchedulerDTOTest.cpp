#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/WeeklySchedulerDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;



TEST(WeeklySchedulerDTOTest, defaultConstructor) {
	WeeklySchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasValues());
	EXPECT_THROW(schedulerDTO.getValues(), logic_error);
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
	const WeeklySchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDTOTest, moveConstructor) {
	const list<bool> expectedValues({ false, true, false, false });

	WeeklySchedulerDTO source(move(list<bool>(expectedValues)));
	const WeeklySchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasValues());
	EXPECT_THAT(schedulerDTO.getValues(), ContainerEq(expectedValues));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(WeeklySchedulerDTO, list<bool>, Values, list<bool>({ false, false, true }));

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerDTOTest, equalsOperator) {
	WeeklySchedulerDTO dto1;
	WeeklySchedulerDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(list<bool>, Values,
			list<bool>({ false, true }),
			list<bool>({ false, true, true })
		);
}
