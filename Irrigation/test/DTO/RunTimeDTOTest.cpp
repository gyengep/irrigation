#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/RunTimeDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(RunTimeDTOTest, defaultConstructor) {
	RunTimeDTO runTimeDTO;

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_FALSE(runTimeDTO.hasMinutes());
	EXPECT_FALSE(runTimeDTO.hasSeconds());

	EXPECT_THROW(runTimeDTO.getId(), logic_error);
	EXPECT_THROW(runTimeDTO.getMinutes(), logic_error);
	EXPECT_THROW(runTimeDTO.getSeconds(), logic_error);
}

TEST(RunTimeDTOTest, parametrizedConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;

	const RunTimeDTO runTimeDTO(expectedMinutes, expectedSeconds);

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
	const RunTimeDTO runTimeDTO(source);

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
	const RunTimeDTO runTimeDTO(move(source));

	EXPECT_FALSE(runTimeDTO.hasId());
	EXPECT_TRUE(runTimeDTO.hasMinutes());
	EXPECT_TRUE(runTimeDTO.hasSeconds());

	EXPECT_THAT(runTimeDTO.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDTO.getSeconds(), Eq(expectedSeconds));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(RunTimeDTO, unsigned, Id, 100);
CHECK_DTO_FUNCTIONS(RunTimeDTO, unsigned, Minutes, 25);
CHECK_DTO_FUNCTIONS(RunTimeDTO, unsigned, Seconds, 48);

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeDTOTest, equalsOperator) {
	RunTimeDTO dto1;
	RunTimeDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 24100, 66);
	CHECK_DTO_EQUALS_COPY(unsigned, Minutes, 58, 37);
	CHECK_DTO_EQUALS_COPY(unsigned, Seconds, 18, 20);
}
