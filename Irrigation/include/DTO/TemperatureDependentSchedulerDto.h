#pragma once
#include "DtoMacros.h"


#define TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS											\
	DTO_MEMBER_COPY(TemperatureDependentSchedulerDto, float, RemainingCorrection);			\
	DTO_MEMBER_COPY(TemperatureDependentSchedulerDto, int, MinAdjustment);					\
	DTO_MEMBER_COPY(TemperatureDependentSchedulerDto, int, MaxAdjustment);					\


CREATE_DTO_CLASS(TemperatureDependentSchedulerDto)

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)

	TemperatureDependentSchedulerDto(
			float RemainingCorrection,
			int MinAdjustment,
			int MaxAdjustment
		)
	{
		TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS;
	}

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_COPY_CTOR_MOVE(CLASS, TYPE, NAME)

	IMPLEMENT_COPY_CTOR(TemperatureDependentSchedulerDto, TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)

	IMPLEMENT_EQUALS_OPERATOR(TemperatureDependentSchedulerDto, TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_MOVE(CLASS, TYPE, NAME)

	TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE
};

#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)

IMPLEMENT_OSS_OPERATOR(TemperatureDependentSchedulerDto, TEMPERATURE_DEPENDENT_SCHEDULER_DTO_MEMBERS);

#undef DTO_MEMBER_INIT
#undef DTO_MEMBER_COPY
#undef DTO_MEMBER_MOVE