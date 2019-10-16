#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/HotWeatherSchedulerDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(HotWeatherSchedulerDTOTest, defaultConstructor) {
	const HotWeatherSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasPeriodInSeconds());
	EXPECT_FALSE(schedulerDTO.hasMinTemperature());

	EXPECT_THROW(schedulerDTO.getPeriodInSeconds(), logic_error);
	EXPECT_THROW(schedulerDTO.getMinTemperature(), logic_error);
}

TEST(HotWeatherSchedulerDTOTest, parametrizedConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	const HotWeatherSchedulerDTO schedulerDTO(expectedPeriodInSeconds, expectedMinTemperature);

	EXPECT_TRUE(schedulerDTO.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDTO.hasMinTemperature());

	EXPECT_THAT(schedulerDTO.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDTO.getMinTemperature(), Eq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerDTOTest, copyConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	const HotWeatherSchedulerDTO source(expectedPeriodInSeconds, expectedMinTemperature);
	const HotWeatherSchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDTO.hasMinTemperature());

	EXPECT_THAT(schedulerDTO.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDTO.getMinTemperature(), Eq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerDTOTest, moveConstructor) {
	const unsigned expectedPeriodInSeconds = 19.0f;
	const float expectedMinTemperature= 7.3f;

	HotWeatherSchedulerDTO source(expectedPeriodInSeconds, expectedMinTemperature);
	const HotWeatherSchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasPeriodInSeconds());
	EXPECT_TRUE(schedulerDTO.hasMinTemperature());

	EXPECT_THAT(schedulerDTO.getPeriodInSeconds(), Eq(expectedPeriodInSeconds));
	EXPECT_THAT(schedulerDTO.getMinTemperature(), Eq(expectedMinTemperature));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(HotWeatherSchedulerDTO, float, MinTemperature, 21.2f);
CHECK_DTO_FUNCTIONS(HotWeatherSchedulerDTO, int, PeriodInSeconds, 15);

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerDTOTest, equalsOperator) {
	HotWeatherSchedulerDTO dto1;
	HotWeatherSchedulerDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(float, MinTemperature, 21.2f, 23.0f);
	CHECK_DTO_EQUALS_COPY(int, PeriodInSeconds, 4, 5);
}
