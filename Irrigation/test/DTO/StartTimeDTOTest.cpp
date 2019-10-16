#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/StartTimeDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(StartTimeDTOTest, defaultConstructor) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_FALSE(startTimeDTO.hasHours());
	EXPECT_FALSE(startTimeDTO.hasMinutes());

	EXPECT_THROW(startTimeDTO.getId(), logic_error);
	EXPECT_THROW(startTimeDTO.getHours(), logic_error);
	EXPECT_THROW(startTimeDTO.getMinutes(), logic_error);
}

TEST(StartTimeDTOTest, parametrizedConstructor) {
	const unsigned expectedHour = 5;
	const unsigned expectedMinute = 30;

	const StartTimeDTO startTimeDTO(expectedHour, expectedMinute);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHours());
	EXPECT_TRUE(startTimeDTO.hasMinutes());

	EXPECT_THAT(startTimeDTO.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDTOTest, copyConstructor) {
	const unsigned expectedHour = 15;
	const unsigned expectedMinute = 55;

	const StartTimeDTO source(expectedHour, expectedMinute);
	const StartTimeDTO startTimeDTO(source);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHours());
	EXPECT_TRUE(startTimeDTO.hasMinutes());

	EXPECT_THAT(startTimeDTO.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDTOTest, moveConstructor) {
	const unsigned expectedHour = 15;
	const unsigned expectedMinute = 55;

	StartTimeDTO source(expectedHour, expectedMinute);
	const StartTimeDTO startTimeDTO(move(source));

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHours());
	EXPECT_TRUE(startTimeDTO.hasMinutes());

	EXPECT_THAT(startTimeDTO.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinutes(), Eq(expectedMinute));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(StartTimeDTO, unsigned, Id, 2100);
CHECK_DTO_FUNCTIONS(StartTimeDTO, unsigned, Hours, 23);
CHECK_DTO_FUNCTIONS(StartTimeDTO, unsigned, Minutes, 48);

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeDTOTest, equalsOperator) {
	StartTimeDTO dto1;
	StartTimeDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 24100, 66);
	CHECK_DTO_EQUALS_COPY(unsigned, Hours, 18, 20);
	CHECK_DTO_EQUALS_COPY(unsigned, Minutes, 58, 37);
}
