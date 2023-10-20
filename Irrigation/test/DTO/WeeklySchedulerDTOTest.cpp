#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/WeeklySchedulerDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;



TEST(WeeklySchedulerDtoTest, defaultConstructor) {
	WeeklySchedulerDto schedulerDto;

	EXPECT_FALSE(schedulerDto.hasValues());
	EXPECT_THROW(schedulerDto.getValues(), logic_error);
}

TEST(WeeklySchedulerDtoTest, parametrizedConstructor) {
	const list<bool> expectedValues({ false, true });
	WeeklySchedulerDto schedulerDto(move(list<bool>(expectedValues)));

	EXPECT_TRUE(schedulerDto.hasValues());
	EXPECT_THAT(schedulerDto.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDtoTest, copyConstructor) {
	const list<bool> expectedValues({ false, true, false, false });

	const WeeklySchedulerDto source(move(list<bool>(expectedValues)));
	const WeeklySchedulerDto schedulerDto(source);

	EXPECT_TRUE(schedulerDto.hasValues());
	EXPECT_THAT(schedulerDto.getValues(), ContainerEq(expectedValues));
}

TEST(WeeklySchedulerDtoTest, moveConstructor) {
	const list<bool> expectedValues({ false, true, false, false });

	WeeklySchedulerDto source(move(list<bool>(expectedValues)));
	const WeeklySchedulerDto schedulerDto(move(source));

	EXPECT_TRUE(schedulerDto.hasValues());
	EXPECT_THAT(schedulerDto.getValues(), ContainerEq(expectedValues));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(WeeklySchedulerDto, list<bool>, Values, list<bool>({ false, false, true }));

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerDtoTest, equalsOperator) {
	WeeklySchedulerDto dto1;
	WeeklySchedulerDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(list<bool>, Values,
			list<bool>({ false, true }),
			list<bool>({ false, true, true })
		);
}
