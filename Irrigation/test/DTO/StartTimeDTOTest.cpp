#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/StartTimeDTO.h"

using namespace std;
using namespace testing;



TEST(StartTimeDTOTest, defaultConstructor) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_FALSE(startTimeDTO.hasHours());
	EXPECT_FALSE(startTimeDTO.hasMinutes());
}

TEST(StartTimeDTOTest, parametrizedConstructor) {
	const unsigned expectedHour = 5;
	const unsigned expectedMinute = 30;
	StartTimeDTO startTimeDTO(expectedHour, expectedMinute);

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

	StartTimeDTO startTimeDTO(source);

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

	StartTimeDTO startTimeDTO(move(source));

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHours());
	EXPECT_TRUE(startTimeDTO.hasMinutes());

	EXPECT_THAT(startTimeDTO.getHours(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDTOTest, hasId) {
	const unsigned expectedId = 2100;
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	startTimeDTO.setId(expectedId);
	EXPECT_TRUE(startTimeDTO.hasId());
}

TEST(StartTimeDTOTest, getId) {
	const unsigned expectedId = 5530;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getId(), logic_error);
	startTimeDTO.setId(expectedId);
	ASSERT_NO_THROW(startTimeDTO.getId());
	EXPECT_THAT(startTimeDTO.getId(), Eq(expectedId));
}

TEST(StartTimeDTOTest, hasHours) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasHours());
	startTimeDTO.setHours(23);
	EXPECT_TRUE(startTimeDTO.hasHours());
}

TEST(StartTimeDTOTest, getHours) {
	const unsigned expectedHour = 21;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getHours(), logic_error);
	startTimeDTO.setHours(expectedHour);
	ASSERT_NO_THROW(startTimeDTO.getHours());
	EXPECT_THAT(startTimeDTO.getHours(), Eq(expectedHour));
}

TEST(StartTimeDTOTest, hasMinutes) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasMinutes());
	startTimeDTO.setMinutes(33);
	EXPECT_TRUE(startTimeDTO.hasMinutes());
}

TEST(StartTimeDTOTest, getMinutes) {
	const unsigned expectedMinute = 51;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getMinutes(), logic_error);
	startTimeDTO.setMinutes(expectedMinute);
	ASSERT_NO_THROW(startTimeDTO.getMinutes());
	EXPECT_THAT(startTimeDTO.getMinutes(), Eq(expectedMinute));
}

TEST(StartTimeDTOTest, equal) {
	StartTimeDTO startTimeDTO1;
	StartTimeDTO startTimeDTO2;

	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);

	const unsigned expectedId1 = 24100;
	const unsigned expectedId2 = 21050;

	startTimeDTO1.setId(expectedId1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setId(expectedId2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setId(expectedId2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);

	const unsigned expectedHour1 = 18;
	const unsigned expectedHour2 = 20;

	startTimeDTO1.setHours(expectedHour1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setHours(expectedHour2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setHours(expectedHour2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);

	const unsigned expectedMinute1 = 58;
	const unsigned expectedMinute2 = 37;

	startTimeDTO1.setMinutes(expectedMinute1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setMinutes(expectedMinute2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setMinutes(expectedMinute2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);
}
