#pragma once
#include "DtoMacros.h"
#include "EveryDaySchedulerDTO.h"
#include "HotWeatherSchedulerDTO.h"
#include "TemperatureDependentSchedulerDTO.h"
#include "WeeklySchedulerDTO.h"


#define SCHEDULERS_DTO_MEMBERS																			\
	DTO_MEMBER_MOVE(SchedulersDTO, EveryDaySchedulerDTO, EveryDayScheduler);							\
	DTO_MEMBER_MOVE(SchedulersDTO, HotWeatherSchedulerDTO, HotWeatherScheduler);						\
	DTO_MEMBER_MOVE(SchedulersDTO, TemperatureDependentSchedulerDTO, TemperatureDependentScheduler);	\
	DTO_MEMBER_MOVE(SchedulersDTO, WeeklySchedulerDTO, WeeklyScheduler)


CREATE_DTO_CLASS(SchedulersDTO)

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)

	SchedulersDTO(
			EveryDaySchedulerDTO&& EveryDayScheduler,
			HotWeatherSchedulerDTO&& HotWeatherScheduler,
			TemperatureDependentSchedulerDTO&& TemperatureDependentScheduler,
			WeeklySchedulerDTO&& WeeklyScheduler
		)
	{
		SCHEDULERS_DTO_MEMBERS;
	}

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_COPY_CTOR_MOVE(CLASS, TYPE, NAME)

	IMPLEMENT_COPY_CTOR(SchedulersDTO, SCHEDULERS_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)

	IMPLEMENT_EQUALS_OPERATOR(SchedulersDTO, SCHEDULERS_DTO_MEMBERS);

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE

	///////////////////////////////////////////////////////////////////////////////

	#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME)
	#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_DECLARE_VALUE_MOVE(CLASS, TYPE, NAME)

	SCHEDULERS_DTO_MEMBERS;

	#undef DTO_MEMBER_INIT
	#undef DTO_MEMBER_COPY
	#undef DTO_MEMBER_MOVE
};

#define DTO_MEMBER_INIT(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_COPY(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)
#define DTO_MEMBER_MOVE(CLASS, TYPE, NAME)			DTO_OSS_OPERATOR(CLASS, TYPE, NAME)

IMPLEMENT_OSS_OPERATOR(SchedulersDTO, SCHEDULERS_DTO_MEMBERS);

#undef DTO_MEMBER_INIT
#undef DTO_MEMBER_COPY
#undef DTO_MEMBER_MOVE
