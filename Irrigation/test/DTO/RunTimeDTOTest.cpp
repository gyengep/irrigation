#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/RunTimeDTO.h"

using namespace std;
using namespace testing;



TEST(RunTimeDTOTest, defaultConstructor) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_FALSE(runTimeDTO.hasMinutes());
	EXPECT_FALSE(runTimeDTO.hasSeconds());
}

TEST(RunTimeDTOTest, parametrizedConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;
	RunTimeDTO runTimeDTO(expectedMinutes, expectedSeconds);

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasMinutes());
	EXPECT_TRUE(runTimeDTO.hasSeconds());

	EXPECT_THAT(runTimeDTO.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDTO.getSeconds(), Eq(expectedSeconds));
}

TEST(RunTimeDTOTest, copyConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;
	const RunTimeDTO source(expectedMinutes, expectedSeconds);

	RunTimeDTO runTimeDTO(source);

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasMinutes());
	EXPECT_TRUE(runTimeDTO.hasSeconds());

	EXPECT_THAT(runTimeDTO.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDTO.getSeconds(), Eq(expectedSeconds));
}

TEST(RunTimeDTOTest, moveConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;
	RunTimeDTO source(expectedMinutes, expectedSeconds);

	RunTimeDTO runTimeDTO(move(source));

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasMinutes());
	EXPECT_TRUE(runTimeDTO.hasSeconds());

	EXPECT_THAT(runTimeDTO.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDTO.getSeconds(), Eq(expectedSeconds));
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

TEST(RunTimeDTOTest, hasMinutes) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasMinutes());
	runTimeDTO.setMinutes(100);
	EXPECT_TRUE(runTimeDTO.hasMinutes());
}

TEST(RunTimeDTOTest, getMinutes) {
	const unsigned expectedMinutes = 25;
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getMinutes(), logic_error);
	runTimeDTO.setMinutes(expectedMinutes);
	ASSERT_NO_THROW(runTimeDTO.getMinutes());
	EXPECT_THAT(runTimeDTO.getMinutes(), Eq(expectedMinutes));
}

TEST(RunTimeDTOTest, hasSeconds) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasSeconds());
	runTimeDTO.setSeconds(50);
	EXPECT_TRUE(runTimeDTO.hasSeconds());
}

TEST(RunTimeDTOTest, getSeconds) {
	const unsigned expectedSeconds = 25;
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getSeconds(), logic_error);
	runTimeDTO.setSeconds(expectedSeconds);
	ASSERT_NO_THROW(runTimeDTO.getSeconds());
	EXPECT_THAT(runTimeDTO.getSeconds(), Eq(expectedSeconds));
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

	const unsigned expectedMinutes1= 5;
	const unsigned expectedMinutes2= 4;

	runTimeDTO1.setMinutes(expectedMinutes1);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO2.setMinutes(expectedMinutes2);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO1.setMinutes(expectedMinutes2);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
	EXPECT_TRUE(runTimeDTO2 == runTimeDTO1);

	const unsigned expectedSeconds1= 30;
	const unsigned expectedSeconds2= 25;

	runTimeDTO1.setSeconds(expectedSeconds1);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO2.setSeconds(expectedSeconds2);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);
	EXPECT_FALSE(runTimeDTO2 == runTimeDTO1);

	runTimeDTO1.setSeconds(expectedSeconds2);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
	EXPECT_TRUE(runTimeDTO2 == runTimeDTO1);
}
