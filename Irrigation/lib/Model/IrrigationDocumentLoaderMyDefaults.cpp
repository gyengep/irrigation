#include "Impl/IrrigationDocumentLoaderMyDefaults.h"
#include "DTO/DocumentDto.h"
#include "Logger/Logger.h"


IrrigationDocumentLoaderMyDefaults::IrrigationDocumentLoaderMyDefaults() {
}

IrrigationDocumentLoaderMyDefaults::~IrrigationDocumentLoaderMyDefaults() {
}

bool IrrigationDocumentLoaderMyDefaults::load(IrrigationDocument& document) {

	const DocumentDto documentDto(
		ProgramDtoList {
			ProgramDto(true, "Fűlocsolás", 100, "temperature-dependent",
					SchedulersDto(
							EveryDaySchedulerDto(),
							HotWeatherSchedulerDto(),
							TemperatureDependentSchedulerDto(0.75f, 50, 0),
							WeeklySchedulerDto()
						),
					RunTimeDtoList({
							RunTimeDto(26, 0),
							RunTimeDto(38, 0),
							RunTimeDto(32, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
						}),
					StartTimeDtoList({
							StartTimeDto(4, 0)
						})
				),
			ProgramDto(true, "Buxus", 100, "temperature-dependent",
					SchedulersDto(
							EveryDaySchedulerDto(),
							HotWeatherSchedulerDto(),
							TemperatureDependentSchedulerDto(1.0f, 100, 0),
							WeeklySchedulerDto()
						),
					RunTimeDtoList({
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(20, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
						}),
					StartTimeDtoList({
							StartTimeDto(6, 0)
						})
				),
			ProgramDto(true, "Kánikula", 100, "hot-weather",
					SchedulersDto(
							EveryDaySchedulerDto(),
							HotWeatherSchedulerDto(2 * 3600, 33),
							TemperatureDependentSchedulerDto(),
							WeeklySchedulerDto()
						),
					RunTimeDtoList({
							RunTimeDto(2, 30),
							RunTimeDto(3, 30),
							RunTimeDto(3, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
						}),
					StartTimeDtoList({
							StartTimeDto(12, 0),
							StartTimeDto(13, 0),
							StartTimeDto(14, 0),
							StartTimeDto(15, 0),
							StartTimeDto(16, 0),
							StartTimeDto(17, 0),
						})
				),
			ProgramDto(true, "Paradicsom", 100, "temperature-dependent",
					SchedulersDto(
							EveryDaySchedulerDto(),
							HotWeatherSchedulerDto(),
							TemperatureDependentSchedulerDto(1.0f, 75, 75),
							WeeklySchedulerDto()
						),
					RunTimeDtoList({
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(1, 0),
							RunTimeDto(0, 0),
						}),
					StartTimeDtoList({
							StartTimeDto(7, 0),
							StartTimeDto(20, 0)
						})
				),
			ProgramDto(true, "Virág", 100, "temperature-dependent",
					SchedulersDto(
							EveryDaySchedulerDto(),
							HotWeatherSchedulerDto(),
							TemperatureDependentSchedulerDto(1.0f, 75, 75),
							WeeklySchedulerDto()
						),
					RunTimeDtoList({
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(0, 0),
							RunTimeDto(16, 0),
						}),
					StartTimeDtoList({
							StartTimeDto(7, 10),
							StartTimeDto(20, 10)
						})
				)
		}
	);

	document.updateFromDocumentDto(documentDto);
	LOGGER.debug("MyDefaults configuration successfully loaded");
	return false;
}
