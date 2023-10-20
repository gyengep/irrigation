#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/StartTimeDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(StartTimeDtoTest, defaultConstructor) {
	StartTimeDto startTimeDto;

	EXPECT_FALSE(startTimeDto.hasId());
	EXPECT_FALSE(startTimeDto.hasHours());
	EXPECT_FALSE(startTimeDto.hasMinutes());

	EXPECT_THROW(startTimeDto.getId(), logic_error);
	EXPECT_THROW(startTimeDto.getHours(), logic_error);
	EXPECT_THROW(startTimeDto.getMinutes(), logic_error);
}

TEST(StartTimeDtoTest, parametrizedConstructor) {
	const unsigned expectedHour = 5;
	const unsigned expectedMinute = 30;

	const StartTimeDto startTimeDto(expectedHour, expectedMinute);

	EXPECT_FALSE(startTimeDto.hasId());
	EXPECT_TRUE(startTimeDto.hasHours());
	EXPECT_TRUE(startTimeDto.hasMinutes());

	EXPECT_THAT(startTimeDto.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDto.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDtoTest, copyConstructor) {
	const unsigned expectedHour = 15;
	const unsigned expectedMinute = 55;

	const StartTimeDto source(expectedHour, expectedMinute);
	const StartTimeDto startTimeDto(source);

	EXPECT_FALSE(startTimeDto.hasId());
	EXPECT_TRUE(startTimeDto.hasHours());
	EXPECT_TRUE(startTimeDto.hasMinutes());

	EXPECT_THAT(startTimeDto.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDto.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDtoTest, moveConstructor) {
	const unsigned expectedHour = 15;
	const unsigned expectedMinute = 55;

	StartTimeDto source(expectedHour, expectedMinute);
	const StartTimeDto startTimeDto(move(source));

	EXPECT_FALSE(startTimeDto.hasId());
	EXPECT_TRUE(startTimeDto.hasHours());
	EXPECT_TRUE(startTimeDto.hasMinutes());

	EXPECT_THAT(startTimeDto.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDto.getMinutes(), Eq(expectedMinute));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(StartTimeDto, unsigned, Id, 2100);
CHECK_DTO_FUNCTIONS(StartTimeDto, unsigned, Hours, 23);
CHECK_DTO_FUNCTIONS(StartTimeDto, unsigned, Minutes, 48);

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeDtoTest, equalsOperator) {
	StartTimeDto dto1;
	StartTimeDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 24100, 66);
	CHECK_DTO_EQUALS_COPY(unsigned, Hours, 18, 20);
	CHECK_DTO_EQUALS_COPY(unsigned, Minutes, 58, 37);
}
