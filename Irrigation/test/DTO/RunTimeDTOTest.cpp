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
	RunTimeDTO runTimeDTO(5);

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasValue());

	EXPECT_THAT(runTimeDTO.getValue(), Eq(5));
}

TEST(RunTimeDTOTest, copyConstructor) {
	RunTimeDTO runTimeDTO(RunTimeDTO(5));

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasValue());

	EXPECT_THAT(runTimeDTO.getValue(), Eq(5));
}

TEST(RunTimeDTOTest, getId) {
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getId(), logic_error);
	runTimeDTO.setId(5);
	ASSERT_NO_THROW(runTimeDTO.getId());
	EXPECT_THAT(runTimeDTO.getId(), Eq(5));
}

TEST(RunTimeDTOTest, getValue) {
	RunTimeDTO runTimeDTO;

	EXPECT_THROW(runTimeDTO.getValue(), logic_error);
	runTimeDTO.setValue(5);
	ASSERT_NO_THROW(runTimeDTO.getValue());
	EXPECT_THAT(runTimeDTO.getValue(), Eq(5));
}

TEST(RunTimeDTOTest, hasId) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasId());
	runTimeDTO.setId(5);
	EXPECT_TRUE(runTimeDTO.hasId());
}

TEST(RunTimeDTOTest, hasValue) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasValue());
	runTimeDTO.setValue(5);
	EXPECT_TRUE(runTimeDTO.hasValue());
}

TEST(RunTimeDTOTest, equal) {
	RunTimeDTO runTimeDTO1;
	RunTimeDTO runTimeDTO2;

	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO1.setId(10);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO2.setId(15);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO1.setId(15);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO1.setValue(20);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO2.setValue(25);
	EXPECT_FALSE(runTimeDTO1 == runTimeDTO2);

	runTimeDTO1.setValue(25);
	EXPECT_TRUE(runTimeDTO1 == runTimeDTO2);
}
