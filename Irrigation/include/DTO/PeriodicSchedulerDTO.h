#pragma once
#include <list>
#include "DtoMacros.h"


#define PERIODIC_SCHEDULER_DTO_MEMBERS													\
	DTO_MEMBER_MOVE(PeriodicSchedulerDTO, std::list<bool>, Values);						\
	DTO_MEMBER_COPY(PeriodicSchedulerDTO, unsigned, PeriodStartYear);					\
	DTO_MEMBER_COPY(PeriodicSchedulerDTO, unsigned, PeriodStartMonth);					\
	DTO_MEMBER_COPY(PeriodicSchedulerDTO, unsigned, PeriodStartDay)


template <class CLASS>
void CopyFunc(CLASS& dst, const CLASS& src) {
	dst = src;
}

CREATE_DTO_CLASS(PeriodicSchedulerDTO)

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)

	PeriodicSchedulerDTO(
			std::list<bool>&& Values,
			unsigned PeriodStartYear,
			unsigned PeriodStartMonth,
			unsigned PeriodStartDay
		)
	{
		PERIODIC_SCHEDULER_DTO_MEMBERS;
	}

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_COPY_CTOR_MOVE(CLASS, TYPE, NAME)

	IMPLEMENT_COPY_CTOR(PeriodicSchedulerDTO, PERIODIC_SCHEDULER_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)

	IMPLEMENT_EQUALS_OPERATOR(PeriodicSchedulerDTO, PERIODIC_SCHEDULER_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_MOVE(CLASS, TYPE, NAME)

	PERIODIC_SCHEDULER_DTO_MEMBERS;

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE
};

#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)

IMPLEMENT_OSS_OPERATOR(PeriodicSchedulerDTO, PERIODIC_SCHEDULER_DTO_MEMBERS);

#undef DTO_MEMBER_INIT
#undef DTO_MEMBER_COPY
#undef DTO_MEMBER_MOVE
