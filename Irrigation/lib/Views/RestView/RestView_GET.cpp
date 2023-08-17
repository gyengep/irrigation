#include "RestView.h"
#include "Model/IrrigationDocument.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include <mutex>


std::list<ProgramDTO> RestView::getProgramDTOList(IrrigationDocument& irrigationDocument) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().toProgramDtoList();
}

std::list<RunTimeDTO> RestView::getRunTimeDTOList(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getRunTimeContainer().toRunTimeDtoList();
}

std::list<StartTimeDTO> RestView::getStartTimeDTOList(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().toStartTimeDtoList();
}

ProgramDTO RestView::getProgramDTO(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->toProgramDto();
}

StartTimeDTO RestView::getStartTimeDTO(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().at(startTimeId)->toStartTimeDto();
}

HotWeatherSchedulerDTO RestView::getHotWeatherSchedulerDTO(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getHotWeatherScheduler().toHotWeatherSchedulerDto();
}

TemperatureDependentSchedulerDTO RestView::getTemperatureDependentSchedulerDTO(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto();
}

WeeklySchedulerDTO RestView::getWeeklySchedulerDTO(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getWeeklyScheduler().toWeeklySchedulerDto();
}
