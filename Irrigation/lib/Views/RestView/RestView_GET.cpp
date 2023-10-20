#include "RestView.h"
#include "Model/IrrigationDocument.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include <mutex>


ProgramDtoList RestView::getProgramDtoList(IrrigationDocument& irrigationDocument) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().toProgramDtoList();
}

RunTimeDtoList RestView::getRunTimeDtoList(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getRunTimeContainer().toRunTimeDtoList();
}

StartTimeDtoList RestView::getStartTimeDtoList(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().toStartTimeDtoList();
}

ProgramDto RestView::getProgramDto(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->toProgramDto();
}

StartTimeDto RestView::getStartTimeDto(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().at(startTimeId)->toStartTimeDto();
}

HotWeatherSchedulerDto RestView::getHotWeatherSchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getHotWeatherScheduler().toHotWeatherSchedulerDto();
}

TemperatureDependentSchedulerDto RestView::getTemperatureDependentSchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto();
}

WeeklySchedulerDto RestView::getWeeklySchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	return irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getWeeklyScheduler().toWeeklySchedulerDto();
}
