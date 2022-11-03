#pragma once
#include <list>
#include <string>
#include "ProgramDTO.h"
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "SchedulersDTO.h"
#include "DtoMacros.h"


#define PROGRAM_DTO_MEMBERS																			\
	DTO_MEMBER_INIT(ProgramDTO, unsigned, Id);														\
	DTO_MEMBER_COPY(ProgramDTO, bool, Enabled);														\
	DTO_MEMBER_COPY(ProgramDTO, std::string, Name);													\
	DTO_MEMBER_COPY(ProgramDTO, unsigned, Adjustment);												\
	DTO_MEMBER_COPY(ProgramDTO, std::string, SchedulerType);										\
	DTO_MEMBER_MOVE(ProgramDTO, SchedulersDTO, Schedulers);											\
	DTO_MEMBER_MOVE(ProgramDTO, std::list<RunTimeDTO>, RunTimes);									\
	DTO_MEMBER_MOVE(ProgramDTO, std::list<StartTimeDTO>, StartTimes)


CREATE_DTO_CLASS(ProgramDTO)
	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)

	ProgramDTO(bool Enabled, const std::string& Name,
			unsigned Adjustment,
			const std::string& SchedulerType,
			SchedulersDTO&& Schedulers,
			std::list<RunTimeDTO>&& RunTimes,
			std::list<StartTimeDTO>&& StartTimes)
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

	IMPLEMENT_COPY_CTOR(ProgramDTO, PROGRAM_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)

	IMPLEMENT_EQUALS_OPERATOR(ProgramDTO, PROGRAM_DTO_MEMBERS);

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

IMPLEMENT_OSS_OPERATOR(ProgramDTO, PROGRAM_DTO_MEMBERS);

#undef DTO_MEMBER_INIT
#undef DTO_MEMBER_COPY
#undef DTO_MEMBER_MOVE
