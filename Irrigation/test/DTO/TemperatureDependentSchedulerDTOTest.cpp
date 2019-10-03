#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "DtoTest.h"

using namespace std;
using namespace testing;


TEST(TemperatureDependentSchedulerDTOTest, defaultConstructor) {
	const TemperatureDependentSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasRemainingA());
	EXPECT_FALSE(schedulerDTO.hasForecastA());
	EXPECT_FALSE(schedulerDTO.hasForecastB());
	EXPECT_FALSE(schedulerDTO.hasHistoryA());
	EXPECT_FALSE(schedulerDTO.hasHistoryB());
	EXPECT_FALSE(schedulerDTO.hasMinAdjustment());
	EXPECT_FALSE(schedulerDTO.hasMaxAdjustment());
	EXPECT_FALSE(schedulerDTO.hasTrim());

	EXPECT_THROW(schedulerDTO.getRemainingA(), logic_error);
	EXPECT_THROW(schedulerDTO.getForecastA(), logic_error);
	EXPECT_THROW(schedulerDTO.getForecastB(), logic_error);
	EXPECT_THROW(schedulerDTO.getHistoryA(), logic_error);
	EXPECT_THROW(schedulerDTO.getHistoryB(), logic_error);
	EXPECT_THROW(schedulerDTO.getMinAdjustment(), logic_error);
	EXPECT_THROW(schedulerDTO.getMaxAdjustment(), logic_error);
	EXPECT_THROW(schedulerDTO.getTrim(), logic_error);
}

TEST(TemperatureDependentSchedulerDTOTest, parametrizedConstructor) {
	const float expectedRemainingA = 19.1f;
	const float expectedForecastA = 7.3f;
	const float expectedForecastB = 9.4f;
	const float expectedHistoryA = 4.7f;
	const float expectedHistoryB = 12.9f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;
	const int expectedTrim = 98;

	const TemperatureDependentSchedulerDTO schedulerDTO(
			expectedRemainingA,
			expectedForecastA, expectedForecastB,
			expectedHistoryA, expectedHistoryB,
			expectedMinAdjustment, expectedMaxAdjustment,
			expectedTrim
		);

	EXPECT_TRUE(schedulerDTO.hasRemainingA());
	EXPECT_TRUE(schedulerDTO.hasForecastA());
	EXPECT_TRUE(schedulerDTO.hasForecastB());
	EXPECT_TRUE(schedulerDTO.hasHistoryA());
	EXPECT_TRUE(schedulerDTO.hasHistoryB());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());
	EXPECT_TRUE(schedulerDTO.hasTrim());

	EXPECT_THAT(schedulerDTO.getRemainingA(), Eq(expectedRemainingA));
	EXPECT_THAT(schedulerDTO.getForecastA(), Eq(expectedForecastA));
	EXPECT_THAT(schedulerDTO.getForecastB(), Eq(expectedForecastB));
	EXPECT_THAT(schedulerDTO.getHistoryA(), Eq(expectedHistoryA));
	EXPECT_THAT(schedulerDTO.getHistoryB(), Eq(expectedHistoryB));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
	EXPECT_THAT(schedulerDTO.getTrim(), Eq(expectedTrim));
}

TEST(TemperatureDependentSchedulerDTOTest, copyConstructor) {
	const float expectedRemainingA = 19.1f;
	const float expectedForecastA = 7.3f;
	const float expectedForecastB = 9.4f;
	const float expectedHistoryA = 4.7f;
	const float expectedHistoryB = 12.9f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;
	const int expectedTrim = 98;

	const TemperatureDependentSchedulerDTO source(
			expectedRemainingA,
			expectedForecastA, expectedForecastB,
			expectedHistoryA, expectedHistoryB,
			expectedMinAdjustment, expectedMaxAdjustment,
			expectedTrim
		);

	const TemperatureDependentSchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasRemainingA());
	EXPECT_TRUE(schedulerDTO.hasForecastA());
	EXPECT_TRUE(schedulerDTO.hasForecastB());
	EXPECT_TRUE(schedulerDTO.hasHistoryA());
	EXPECT_TRUE(schedulerDTO.hasHistoryB());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());
	EXPECT_TRUE(schedulerDTO.hasTrim());

	EXPECT_THAT(schedulerDTO.getRemainingA(), Eq(expectedRemainingA));
	EXPECT_THAT(schedulerDTO.getForecastA(), Eq(expectedForecastA));
	EXPECT_THAT(schedulerDTO.getForecastB(), Eq(expectedForecastB));
	EXPECT_THAT(schedulerDTO.getHistoryA(), Eq(expectedHistoryA));
	EXPECT_THAT(schedulerDTO.getHistoryB(), Eq(expectedHistoryB));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
	EXPECT_THAT(schedulerDTO.getTrim(), Eq(expectedTrim));
}

TEST(TemperatureDependentSchedulerDTOTest, moveConstructor) {
	const float expectedRemainingA = 19.1f;
	const float expectedForecastA = 7.3f;
	const float expectedForecastB = 9.4f;
	const float expectedHistoryA = 4.7f;
	const float expectedHistoryB = 12.9f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;
	const int expectedTrim = 98;

	TemperatureDependentSchedulerDTO source(
			expectedRemainingA,
			expectedForecastA, expectedForecastB,
			expectedHistoryA, expectedHistoryB,
			expectedMinAdjustment, expectedMaxAdjustment,
			expectedTrim
		);

	const TemperatureDependentSchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasRemainingA());
	EXPECT_TRUE(schedulerDTO.hasForecastA());
	EXPECT_TRUE(schedulerDTO.hasForecastB());
	EXPECT_TRUE(schedulerDTO.hasHistoryA());
	EXPECT_TRUE(schedulerDTO.hasHistoryB());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());
	EXPECT_TRUE(schedulerDTO.hasTrim());

	EXPECT_THAT(schedulerDTO.getRemainingA(), Eq(expectedRemainingA));
	EXPECT_THAT(schedulerDTO.getForecastA(), Eq(expectedForecastA));
	EXPECT_THAT(schedulerDTO.getForecastB(), Eq(expectedForecastB));
	EXPECT_THAT(schedulerDTO.getHistoryA(), Eq(expectedHistoryA));
	EXPECT_THAT(schedulerDTO.getHistoryB(), Eq(expectedHistoryB));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
	EXPECT_THAT(schedulerDTO.getTrim(), Eq(expectedTrim));
}


CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, RemainingA, 21.2f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, ForecastA, 25.2f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, ForecastB, 18.32f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, HistoryA, 15.2f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, HistoryB, 8.32f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, int, MinAdjustment, 23);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, int, MaxAdjustment, 46);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, int, Trim, 2);


TEST(TemperatureDependentSchedulerDTOTest, equalsOperator) {
	TemperatureDependentSchedulerDTO dto1;
	TemperatureDependentSchedulerDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS(float, RemainingA, 25.3, 35.1);
	CHECK_DTO_EQUALS(float, ForecastA, 25.3, 35.1);
	CHECK_DTO_EQUALS(float, ForecastB, 35.3, 25.1);
	CHECK_DTO_EQUALS(float, HistoryA, 45.3, 15.1);
	CHECK_DTO_EQUALS(float, HistoryB, 55.3, 5.1);
	CHECK_DTO_EQUALS(int, MinAdjustment, 10, 20);
	CHECK_DTO_EQUALS(int, MaxAdjustment, 15, 18);
	CHECK_DTO_EQUALS(int, Trim, 4, 5);
}
