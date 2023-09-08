#include "IrrigationDocumentLoaderMyDefaults.h"
#include "DTO/DocumentDTO.h"
#include "Logger/Logger.h"


IrrigationDocumentLoaderMyDefaults::IrrigationDocumentLoaderMyDefaults() {
}

IrrigationDocumentLoaderMyDefaults::~IrrigationDocumentLoaderMyDefaults() {
}

bool IrrigationDocumentLoaderMyDefaults::load(IrrigationDocument& document) {

	const DocumentDTO documentDto(
		std::list<ProgramDTO> {
			ProgramDTO(true, "Fűlocsolás", 100, "temperature-dependent",
					SchedulersDTO(
							EveryDaySchedulerDTO(),
							HotWeatherSchedulerDTO(),
							TemperatureDependentSchedulerDTO(0.75f, 50, 0),
							WeeklySchedulerDTO()
						),
					std::list<RunTimeDTO>({
							RunTimeDTO(26, 0),
							RunTimeDTO(38, 0),
							RunTimeDTO(32, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
						}),
					std::list<StartTimeDTO>({
							StartTimeDTO(4, 0)
						})
				),
			ProgramDTO(true, "Buxus", 100, "temperature-dependent",
					SchedulersDTO(
							EveryDaySchedulerDTO(),
							HotWeatherSchedulerDTO(),
							TemperatureDependentSchedulerDTO(1.0f, 100, 0),
							WeeklySchedulerDTO()
						),
					std::list<RunTimeDTO>({
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(20, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
						}),
					std::list<StartTimeDTO>({
							StartTimeDTO(6, 0)
						})
				),
			ProgramDTO(true, "Kánikula", 100, "hot-weather",
					SchedulersDTO(
							EveryDaySchedulerDTO(),
							HotWeatherSchedulerDTO(2 * 3600, 33),
							TemperatureDependentSchedulerDTO(),
							WeeklySchedulerDTO()
						),
					std::list<RunTimeDTO>({
							RunTimeDTO(2, 30),
							RunTimeDTO(3, 30),
							RunTimeDTO(3, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
						}),
					std::list<StartTimeDTO>({
							StartTimeDTO(12, 0),
							StartTimeDTO(13, 0),
							StartTimeDTO(14, 0),
							StartTimeDTO(15, 0),
							StartTimeDTO(16, 0),
							StartTimeDTO(17, 0),
						})
				),
			ProgramDTO(true, "Paradicsom", 100, "temperature-dependent",
					SchedulersDTO(
							EveryDaySchedulerDTO(),
							HotWeatherSchedulerDTO(),
							TemperatureDependentSchedulerDTO(1.0f, 75, 75),
							WeeklySchedulerDTO()
						),
					std::list<RunTimeDTO>({
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(1, 0),
							RunTimeDTO(0, 0),
						}),
					std::list<StartTimeDTO>({
							StartTimeDTO(7, 0),
							StartTimeDTO(20, 0)
						})
				),
			ProgramDTO(true, "Virág", 100, "temperature-dependent",
					SchedulersDTO(
							EveryDaySchedulerDTO(),
							HotWeatherSchedulerDTO(),
							TemperatureDependentSchedulerDTO(1.0f, 75, 75),
							WeeklySchedulerDTO()
						),
					std::list<RunTimeDTO>({
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(0, 0),
							RunTimeDTO(16, 0),
						}),
					std::list<StartTimeDTO>({
							StartTimeDTO(7, 10),
							StartTimeDTO(20, 10)
						})
				)
		}
	);

	document.updateFromDocumentDto(documentDto);
	LOGGER.debug("MyDefaults configuration successfully loaded");
	return false;
}
