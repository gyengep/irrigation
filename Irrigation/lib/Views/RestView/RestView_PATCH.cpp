#include "RestView.h"
#include "Model/IrrigationDocument.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include <mutex>


std::string RestView::patchProgram(IrrigationDocument& irrigationDocument, const IdType& programId, const ProgramDto& programDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	ProgramPtr program = irrigationDocument.getProgramContainer().at(programId);

	irrigationDocument.setModified();
	program->updateFromProgramDto(programDto);

	return program->toString();
}

std::string RestView::patchRunTimeList(IrrigationDocument& irrigationDocument, const IdType& programId, const std::list<RunTimeDto>& runTimeDtoList) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	RunTimeContainer& runTimes = irrigationDocument.getProgramContainer().at(programId)->getRunTimeContainer();

	irrigationDocument.setModified();
	runTimes.updateFromRunTimeDtoList(runTimeDtoList);

	return runTimes.toString();
}

std::string RestView::patchStartTime(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId, const StartTimeDto& startTimeDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	StartTimePtr startTime = irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().at(startTimeId);

	irrigationDocument.setModified();
	startTime->updateFromStartTimeDto(startTimeDto);

	return startTime->toString();
}

std::string RestView::patchHotWeatherScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const HotWeatherSchedulerDto& hotWeatherSchedulerDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	HotWeatherScheduler& hotWeatherScheduler = irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getHotWeatherScheduler();

	irrigationDocument.setModified();
	hotWeatherScheduler.updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

	return hotWeatherScheduler.toString();
}

std::string RestView::patchTemperatureDependentScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const TemperatureDependentSchedulerDto& temperatureDependentSchedulerDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	TemperatureDependentScheduler& temperatureDependentScheduler = irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getTemperatureDependentScheduler();

	irrigationDocument.setModified();
	temperatureDependentScheduler.updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	return temperatureDependentScheduler.toString();
}

std::string RestView::patchWeeklyScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const WeeklySchedulerDto& weeklySchedulerDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	WeeklyScheduler& weeklyScheduler = irrigationDocument.getProgramContainer().at(programId)->getSchedulerContainer().getWeeklyScheduler();

	irrigationDocument.setModified();
	weeklyScheduler.updateFromWeeklySchedulerDto(weeklySchedulerDto);

	return weeklyScheduler.toString();
}
