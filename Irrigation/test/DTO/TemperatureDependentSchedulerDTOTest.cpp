#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/TemperatureDependentSchedulerDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(TemperatureDependentSchedulerDtoTest, defaultConstructor) {
	const TemperatureDependentSchedulerDto schedulerDto;

	EXPECT_FALSE(schedulerDto.hasRemainingCorrection());
	EXPECT_FALSE(schedulerDto.hasMinAdjustment());
	EXPECT_FALSE(schedulerDto.hasMaxAdjustment());

	EXPECT_THROW(schedulerDto.getRemainingCorrection(), logic_error);
	EXPECT_THROW(schedulerDto.getMinAdjustment(), logic_error);
	EXPECT_THROW(schedulerDto.getMaxAdjustment(), logic_error);
}

TEST(TemperatureDependentSchedulerDtoTest, parametrizedConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	const TemperatureDependentSchedulerDto schedulerDto(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	EXPECT_TRUE(schedulerDto.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDto.hasMinAdjustment());
	EXPECT_TRUE(schedulerDto.hasMaxAdjustment());

	EXPECT_THAT(schedulerDto.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDto.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDto.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

TEST(TemperatureDependentSchedulerDtoTest, copyConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	const TemperatureDependentSchedulerDto source(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	const TemperatureDependentSchedulerDto schedulerDto(source);

	EXPECT_TRUE(schedulerDto.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDto.hasMinAdjustment());
	EXPECT_TRUE(schedulerDto.hasMaxAdjustment());

	EXPECT_THAT(schedulerDto.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDto.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDto.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

TEST(TemperatureDependentSchedulerDtoTest, moveConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	TemperatureDependentSchedulerDto source(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	const TemperatureDependentSchedulerDto schedulerDto(move(source));

	EXPECT_TRUE(schedulerDto.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDto.hasMinAdjustment());
	EXPECT_TRUE(schedulerDto.hasMaxAdjustment());

	EXPECT_THAT(schedulerDto.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDto.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDto.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDto, float, RemainingCorrection, 21.2f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDto, int, MinAdjustment, 23);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDto, int, MaxAdjustment, 46);

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerDtoTest, equalsOperator) {
	TemperatureDependentSchedulerDto dto1;
	TemperatureDependentSchedulerDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(float, RemainingCorrection, 25.3, 35.1);
	CHECK_DTO_EQUALS_COPY(int, MinAdjustment, 10, 20);
	CHECK_DTO_EQUALS_COPY(int, MaxAdjustment, 15, 18);
}
