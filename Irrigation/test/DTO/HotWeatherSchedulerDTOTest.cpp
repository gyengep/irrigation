#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/HotWeatherSchedulerDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(HotWeatherSchedulerDtoTest, defaultConstructor) {
	const HotWeatherSchedulerDto schedulerDto;

	EXPECT_FALSE(schedulerDto.hasPeriodInSeconds());
	EXPECT_FALSE(schedulerDto.hasMinTemperature());

	EXPECT_THROW(schedulerDto.getPeriodInSeconds(), logic_error);
	EXPECT_THROW(schedulerDto.getMinTemperature(), logic_error);
}

TEST(HotWeatherSchedulerDtoTest, parametrizedConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	const HotWeatherSchedulerDto schedulerDto(expectedPeriodInSeconds, expectedMinTemperature);

	EXPECT_TRUE(schedulerDto.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDto.hasMinTemperature());

	EXPECT_THAT(schedulerDto.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDto.getMinTemperature(), Eq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerDtoTest, copyConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	const HotWeatherSchedulerDto source(expectedPeriodInSeconds, expectedMinTemperature);
	const HotWeatherSchedulerDto schedulerDto(source);

	EXPECT_TRUE(schedulerDto.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDto.hasMinTemperature());

	EXPECT_THAT(schedulerDto.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDto.getMinTemperature(), Eq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerDtoTest, moveConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	HotWeatherSchedulerDto source(expectedPeriodInSeconds, expectedMinTemperature);
	const HotWeatherSchedulerDto schedulerDto(move(source));

	EXPECT_TRUE(schedulerDto.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDto.hasMinTemperature());

	EXPECT_THAT(schedulerDto.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDto.getMinTemperature(), Eq(expectedMinTemperature));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(HotWeatherSchedulerDto, float, MinTemperature, 21.2f);
CHECK_DTO_FUNCTIONS(HotWeatherSchedulerDto, int, PeriodInSeconds, 15);

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerDtoTest, equalsOperator) {
	HotWeatherSchedulerDto dto1;
	HotWeatherSchedulerDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(float, MinTemperature, 21.2f, 23.0f);
	CHECK_DTO_EQUALS_COPY(int, PeriodInSeconds, 4, 5);
}
