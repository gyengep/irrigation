#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/RunTimeDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(RunTimeDtoTest, defaultConstructor) {
	RunTimeDto runTimeDto;

	EXPECT_FALSE(runTimeDto.hasId());
	EXPECT_FALSE(runTimeDto.hasMinutes());
	EXPECT_FALSE(runTimeDto.hasSeconds());

	EXPECT_THROW(runTimeDto.getId(), logic_error);
	EXPECT_THROW(runTimeDto.getMinutes(), logic_error);
	EXPECT_THROW(runTimeDto.getSeconds(), logic_error);
}

TEST(RunTimeDtoTest, parametrizedConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;

	const RunTimeDto runTimeDto(expectedMinutes, expectedSeconds);

	EXPECT_FALSE(runTimeDto.hasId());
	EXPECT_TRUE(runTimeDto.hasMinutes());
	EXPECT_TRUE(runTimeDto.hasSeconds());

	EXPECT_THAT(runTimeDto.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDto.getSeconds(), Eq(expectedSeconds));
}

TEST(RunTimeDtoTest, copyConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;

	const RunTimeDto source(expectedMinutes, expectedSeconds);
	const RunTimeDto runTimeDto(source);

	EXPECT_FALSE(runTimeDto.hasId());
	EXPECT_TRUE(runTimeDto.hasMinutes());
	EXPECT_TRUE(runTimeDto.hasSeconds());

	EXPECT_THAT(runTimeDto.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDto.getSeconds(), Eq(expectedSeconds));
}

TEST(RunTimeDtoTest, moveConstructor) {
	const unsigned expectedMinutes = 5;
	const unsigned expectedSeconds = 30;

	RunTimeDto source(expectedMinutes, expectedSeconds);
	const RunTimeDto runTimeDto(move(source));

	EXPECT_FALSE(runTimeDto.hasId());
	EXPECT_TRUE(runTimeDto.hasMinutes());
	EXPECT_TRUE(runTimeDto.hasSeconds());

	EXPECT_THAT(runTimeDto.getMinutes(), Eq(expectedMinutes));
	EXPECT_THAT(runTimeDto.getSeconds(), Eq(expectedSeconds));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(RunTimeDto, unsigned, Id, 100);
CHECK_DTO_FUNCTIONS(RunTimeDto, unsigned, Minutes, 25);
CHECK_DTO_FUNCTIONS(RunTimeDto, unsigned, Seconds, 48);

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeDtoTest, equalsOperator) {
	RunTimeDto dto1;
	RunTimeDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 24100, 66);
	CHECK_DTO_EQUALS_COPY(unsigned, Minutes, 58, 37);
	CHECK_DTO_EQUALS_COPY(unsigned, Seconds, 18, 20);
}
