#pragma once
#include "DTO/DTO.h"


#define CHECK_DTO_FUNCTIONS(CLASS, TYPE, NAME, VALUE)			\
TEST(CAT(CLASS, Test), HAS_FUNCTION(NAME)) {					\
	CLASS dto;													\
	EXPECT_FALSE(dto.HAS_FUNCTION(NAME)());						\
	dto.SET_FUNCTION(NAME)(VALUE);								\
	EXPECT_TRUE(dto.HAS_FUNCTION(NAME)());						\
}																\
																\
TEST(CAT(CLASS, Test), GET_FUNCTION(NAME)) {					\
	CLASS dto;													\
	EXPECT_THROW(dto.GET_FUNCTION(NAME)(), logic_error);		\
	dto.SET_FUNCTION(NAME)(VALUE);								\
	ASSERT_NO_THROW(dto.GET_FUNCTION(NAME)());					\
	EXPECT_THAT(dto.GET_FUNCTION(NAME)(), Eq(VALUE));			\
}


#define CHECK_DTO_EQUALS(TYPE, NAME, VALUE1, VALUE2)			\
{																\
	const TYPE CAT(NAME, 1) = VALUE1;							\
	const TYPE CAT(NAME, 2) = VALUE2;							\
	dto1.SET_FUNCTION(NAME)(CAT(NAME, 1));						\
	EXPECT_FALSE(dto1 == dto2);									\
	EXPECT_FALSE(dto2 == dto1);									\
	dto2.SET_FUNCTION(NAME)(CAT(NAME, 2));						\
	EXPECT_FALSE(dto1 == dto2);									\
	EXPECT_FALSE(dto2 == dto1);									\
	dto1.SET_FUNCTION(NAME)(CAT(NAME, 2));						\
	EXPECT_TRUE(dto1 == dto2);									\
	EXPECT_TRUE(dto2 == dto1);									\
}

