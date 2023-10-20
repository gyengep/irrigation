#pragma once
#include <list>
#include <string>
#include "ProgramDto.h"
#include "RunTimeDto.h"
#include "StartTimeDto.h"
#include "SchedulersDto.h"
#include "DtoMacros.h"


#define PROGRAM_DTO_MEMBERS																			\
	DTO_MEMBER_INIT(ProgramDto, unsigned, Id);														\
	DTO_MEMBER_COPY(ProgramDto, bool, Enabled);														\
	DTO_MEMBER_COPY(ProgramDto, std::string, Name);													\
	DTO_MEMBER_COPY(ProgramDto, unsigned, Adjustment);												\
	DTO_MEMBER_COPY(ProgramDto, std::string, SchedulerType);										\
	DTO_MEMBER_MOVE(ProgramDto, SchedulersDto, Schedulers);											\
	DTO_MEMBER_MOVE(ProgramDto, RunTimeDtoList, RunTimes);									\
	DTO_MEMBER_MOVE(ProgramDto, StartTimeDtoList, StartTimes)


CREATE_DTO_CLASS(ProgramDto)
	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)

	ProgramDto(bool Enabled, const std::string& Name,
			unsigned Adjustment,
			const std::string& SchedulerType,
			SchedulersDto&& Schedulers,
			RunTimeDtoList&& RunTimes,
			StartTimeDtoList&& StartTimes)
	{
		PROGRAM_DTO_MEMBERS;
	}
	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_COPY_CTOR_MOVE(CLASS, TYPE, NAME)

	IMPLEMENT_COPY_CTOR(ProgramDto, PROGRAM_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)

	IMPLEMENT_EQUALS_OPERATOR(ProgramDto, PROGRAM_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_MOVE(CLASS, TYPE, NAME)

	PROGRAM_DTO_MEMBERS;

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE
};

#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)

IMPLEMENT_OSS_OPERATOR(ProgramDto, PROGRAM_DTO_MEMBERS);

#undef DTO_MEMBER_INIT
#undef DTO_MEMBER_COPY
#undef DTO_MEMBER_MOVE

typedef std::list<ProgramDto> ProgramDtoList;
