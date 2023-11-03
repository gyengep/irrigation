#pragma once
#include "DTO/ProgramDto.h"
#include "SchedulersDtoSamples.h"
#include "RunTimeDtoListSamples.h"
#include "StartTimeDtoListSamples.h"


namespace DtoSamples {

	const ProgramDto programDtoSample1(true, "program1", 215, "hot-weather",
			SchedulersDto(schedulersDtoSample1),
			RunTimeDtoList(runTimeDtoListSample1),
			StartTimeDtoList(startTimeDtoListSample1)
		);

	const ProgramDto programDtoSample2(false, "program2", 216, "temperature-dependent",
			SchedulersDto(schedulersDtoSample2),
			RunTimeDtoList(runTimeDtoListSample2),
			StartTimeDtoList(startTimeDtoListSample2)
		);

	const ProgramDto programDtoSample3(true, "program3", 217, "weekly",
			SchedulersDto(schedulersDtoSample3),
			RunTimeDtoList(runTimeDtoListSample3),
			StartTimeDtoList(startTimeDtoListSample3)
		);

}
