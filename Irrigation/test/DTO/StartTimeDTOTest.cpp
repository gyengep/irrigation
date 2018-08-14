#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/StartTimeDTO.h"

using namespace std;
using namespace testing;



TEST(StartTimeDTOTest, defaultConstructor) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_FALSE(startTimeDTO.hasHour());
	EXPECT_FALSE(startTimeDTO.hasMinute());
}

TEST(StartTimeDTOTest, constructor) {
	const unsigned expectedHour = 5;
	const unsigned expectedMinute = 30;
	StartTimeDTO startTimeDTO(expectedHour, expectedMinute);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHour());
	EXPECT_TRUE(startTimeDTO.hasMinute());

	EXPECT_THAT(startTimeDTO.getHour(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinute(), Eq(expectedMinute));
}

TEST(StartTimeDTOTest, copyConstructor) {
	const unsigned expectedHour = 15;
	const unsigned expectedMinute = 55;
	const StartTimeDTO other(expectedHour, expectedMinute);

	StartTimeDTO startTimeDTO(other);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasHour());
	EXPECT_TRUE(startTimeDTO.hasMinute());

	EXPECT_THAT(startTimeDTO.getHour(), Eq(expectedHour));
	EXPECT_THAT(startTimeDTO.getMinute(), Eq(expectedMinute));
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

TEST(StartTimeDTOTest, hasHour) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasHour());
	startTimeDTO.setHour(23);
	EXPECT_TRUE(startTimeDTO.hasHour());
}

TEST(StartTimeDTOTest, getHour) {
	const unsigned expectedHour = 21;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getHour(), logic_error);
	startTimeDTO.setHour(expectedHour);
	ASSERT_NO_THROW(startTimeDTO.getHour());
	EXPECT_THAT(startTimeDTO.getHour(), Eq(expectedHour));
}

TEST(StartTimeDTOTest, hasMinute) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasMinute());
	startTimeDTO.setMinute(33);
	EXPECT_TRUE(startTimeDTO.hasMinute());
}

TEST(StartTimeDTOTest, getMinute) {
	const unsigned expectedMinute = 51;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getMinute(), logic_error);
	startTimeDTO.setMinute(expectedMinute);
	ASSERT_NO_THROW(startTimeDTO.getMinute());
	EXPECT_THAT(startTimeDTO.getMinute(), Eq(expectedMinute));
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

	startTimeDTO1.setHour(expectedHour1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setHour(expectedHour2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setHour(expectedHour2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);

	const unsigned expectedMinute1 = 58;
	const unsigned expectedMinute2 = 37;

	startTimeDTO1.setMinute(expectedMinute1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setMinute(expectedMinute2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setMinute(expectedMinute2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);
}
