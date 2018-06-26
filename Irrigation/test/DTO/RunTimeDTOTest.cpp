#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/RunTimeDTO.h"

using namespace std;
using namespace testing;



TEST(RunTimeDTOTest, defaultConstructor) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_FALSE(runTimeDTO.hasValue());
}

TEST(RunTimeDTOTest, constructor) {
	const unsigned expectedValue = 5;
	RunTimeDTO runTimeDTO(expectedValue);

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasValue());

	EXPECT_THAT(runTimeDTO.getValue(), Eq(expectedValue));
}

TEST(RunTimeDTOTest, copyConstructor) {
	const unsigned expectedValue = 5;
	const RunTimeDTO other(expectedValue);

	RunTimeDTO runTimeDTO(other);

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasValue());

	EXPECT_THAT(runTimeDTO.getValue(), Eq(expectedValue));
}

TEST(RunTimeDTOTest, hasId) {
	const unsigned expectedId = 100;
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasId());
	runTimeDTO.setId(expectedId);
	EXPECT_TRUE(runTimeDTO.hasId());
}

TEST(RunTimeDTOTest, getId) {
	const unsigned expectedId = 30;
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getId(), logic_error);
	runTimeDTO.setId(expectedId);
	ASSERT_NO_THROW(runTimeDTO.getId());
	EXPECT_THAT(runTimeDTO.getId(), Eq(expectedId));
}

TEST(RunTimeDTOTest, hasValue) {
	const unsigned expectedValue = 150;
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasValue());
	runTimeDTO.setValue(expectedValue);
	EXPECT_TRUE(runTimeDTO.hasValue());
}

TEST(RunTimeDTOTest, getValue) {
	const unsigned expectedValue = 25;
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getValue(), logic_error);
	runTimeDTO.setValue(expectedValue);
	ASSERT_NO_THROW(runTimeDTO.getValue());
	EXPECT_THAT(runTimeDTO.getValue(), Eq(expectedValue));
}

TEST(RunTimeDTOTest, equal) {
	RunTimeDTO runTimeDTO1;
	RunTimeDTO runTimeDTO2;

	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
	EXPECT_TRUE(runTimeDTO2 == runTimeDTO1);

	const unsigned expectedId1 = 100;
	const unsigned expectedId2 = 200;

	runTimeDTO1.setId(expectedId1);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO2.setId(expectedId2);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO1.setId(expectedId2);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
	EXPECT_TRUE(runTimeDTO2 == runTimeDTO1);

	const unsigned expectedValue1 = 56354;
	const unsigned expectedValue2 = 5222;

	runTimeDTO1.setValue(expectedValue1);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO2.setValue(expectedValue2);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO1.setValue(expectedValue2);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
	EXPECT_TRUE(runTimeDTO2 == runTimeDTO1);
}
