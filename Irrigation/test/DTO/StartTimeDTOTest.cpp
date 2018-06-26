#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/StartTimeDTO.h"

using namespace std;
using namespace testing;



TEST(StartTimeDTOTest, defaultConstructor) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_FALSE(startTimeDTO.hasValue());
}

TEST(StartTimeDTOTest, constructor) {
	const unsigned expectedValue = 1223;
	StartTimeDTO startTimeDTO(expectedValue);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasValue());

	EXPECT_THAT(startTimeDTO.getValue(), Eq(expectedValue));
}

TEST(StartTimeDTOTest, copyConstructor) {
	const unsigned expectedValue = 987;
	const StartTimeDTO other(expectedValue);

	StartTimeDTO startTimeDTO(other);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasValue());

	EXPECT_THAT(startTimeDTO.getValue(), Eq(expectedValue));
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

TEST(StartTimeDTOTest, hasValue) {
	const unsigned expectedValue = 2545;
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasValue());
	startTimeDTO.setValue(expectedValue);
	EXPECT_TRUE(startTimeDTO.hasValue());
}

TEST(StartTimeDTOTest, getValue) {
	const unsigned expectedValue = 2545;
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getValue(), logic_error);
	startTimeDTO.setValue(expectedValue);
	ASSERT_NO_THROW(startTimeDTO.getValue());
	EXPECT_THAT(startTimeDTO.getValue(), Eq(expectedValue));
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

	const unsigned expectedValue1 = 24300;
	const unsigned expectedValue2 = 1050;

	startTimeDTO1.setValue(expectedValue1);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO2.setValue(expectedValue2);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);
	EXPECT_FALSE(startTimeDTO2 == startTimeDTO1);

	startTimeDTO1.setValue(expectedValue2);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
	EXPECT_TRUE(startTimeDTO2 == startTimeDTO1);
}
