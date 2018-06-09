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
	StartTimeDTO startTimeDTO(5);

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasValue());

	EXPECT_THAT(startTimeDTO.getValue(), Eq(5));
}

TEST(StartTimeDTOTest, copyConstructor) {
	StartTimeDTO startTimeDTO(StartTimeDTO(5));

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasValue());

	EXPECT_THAT(startTimeDTO.getValue(), Eq(5));
}

TEST(StartTimeDTOTest, getId) {
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getId(), logic_error);
	startTimeDTO.setId(5);
	ASSERT_NO_THROW(startTimeDTO.getId());
	EXPECT_THAT(startTimeDTO.getId(), Eq(5));
}

TEST(StartTimeDTOTest, getValue) {
	StartTimeDTO startTimeDTO;

	EXPECT_THROW(startTimeDTO.getValue(), logic_error);
	startTimeDTO.setValue(5);
	ASSERT_NO_THROW(startTimeDTO.getValue());
	EXPECT_THAT(startTimeDTO.getValue(), Eq(5));
}

TEST(StartTimeDTOTest, hasId) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasId());
	startTimeDTO.setId(5);
	EXPECT_TRUE(startTimeDTO.hasId());
}

TEST(StartTimeDTOTest, hasValue) {
	StartTimeDTO startTimeDTO;

	EXPECT_FALSE(startTimeDTO.hasValue());
	startTimeDTO.setValue(5);
	EXPECT_TRUE(startTimeDTO.hasValue());
}

TEST(StartTimeDTOTest, equal) {
	StartTimeDTO startTimeDTO1;
	StartTimeDTO startTimeDTO2;

	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO1.setId(10);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO2.setId(15);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO1.setId(15);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO1.setValue(20);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO2.setValue(25);
	EXPECT_FALSE(startTimeDTO1 == startTimeDTO2);

	startTimeDTO1.setValue(25);
	EXPECT_TRUE(startTimeDTO1 == startTimeDTO2);
}
