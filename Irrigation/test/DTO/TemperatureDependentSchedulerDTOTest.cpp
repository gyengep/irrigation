#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(TemperatureDependentSchedulerDTOTest, defaultConstructor) {
	const TemperatureDependentSchedulerDTO schedulerDTO;

	EXPECT_FALSE(schedulerDTO.hasRemainingCorrection());
	EXPECT_FALSE(schedulerDTO.hasMinAdjustment());
	EXPECT_FALSE(schedulerDTO.hasMaxAdjustment());

	EXPECT_THROW(schedulerDTO.getRemainingCorrection(), logic_error);
	EXPECT_THROW(schedulerDTO.getMinAdjustment(), logic_error);
	EXPECT_THROW(schedulerDTO.getMaxAdjustment(), logic_error);
}

TEST(TemperatureDependentSchedulerDTOTest, parametrizedConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	const TemperatureDependentSchedulerDTO schedulerDTO(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	EXPECT_TRUE(schedulerDTO.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());

	EXPECT_THAT(schedulerDTO.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

TEST(TemperatureDependentSchedulerDTOTest, copyConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	const TemperatureDependentSchedulerDTO source(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	const TemperatureDependentSchedulerDTO schedulerDTO(source);

	EXPECT_TRUE(schedulerDTO.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());

	EXPECT_THAT(schedulerDTO.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

TEST(TemperatureDependentSchedulerDTOTest, moveConstructor) {
	const float expectedRemainingCorrection = 19.1f;
	const int expectedMinAdjustment = 33;
	const int expectedMaxAdjustment = 76;

	TemperatureDependentSchedulerDTO source(
			expectedRemainingCorrection,
			expectedMinAdjustment, expectedMaxAdjustment
		);

	const TemperatureDependentSchedulerDTO schedulerDTO(move(source));

	EXPECT_TRUE(schedulerDTO.hasRemainingCorrection());
	EXPECT_TRUE(schedulerDTO.hasMinAdjustment());
	EXPECT_TRUE(schedulerDTO.hasMaxAdjustment());

	EXPECT_THAT(schedulerDTO.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(schedulerDTO.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(schedulerDTO.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, float, RemainingCorrection, 21.2f);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, int, MinAdjustment, 23);
CHECK_DTO_FUNCTIONS(TemperatureDependentSchedulerDTO, int, MaxAdjustment, 46);

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerDTOTest, equalsOperator) {
	TemperatureDependentSchedulerDTO dto1;
	TemperatureDependentSchedulerDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(float, RemainingCorrection, 25.3, 35.1);
	CHECK_DTO_EQUALS_COPY(int, MinAdjustment, 10, 20);
	CHECK_DTO_EQUALS_COPY(int, MaxAdjustment, 15, 18);
}
