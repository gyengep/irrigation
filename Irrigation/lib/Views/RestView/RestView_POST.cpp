#include "RestView.h"
#include "Model/IrrigationDocument.h"
#include "Logic/ProgramContainer.h"
#include "Logic/StartTimeContainer.h"
#include <mutex>
#include <utility>


std::pair<IdType, std::string> RestView::postProgramList(IrrigationDocument& irrigationDocument, const ProgramDTO& programDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);

	irrigationDocument.setModified();

	const auto result = irrigationDocument.getProgramContainer().createFromProgramDto(programDto);
	return std::make_pair(result.first, result.second->toString());
}

std::pair<IdType, std::string> RestView::postStartTimeList(IrrigationDocument& irrigationDocument, const IdType& programId, const StartTimeDTO& startTimeDto) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);

	irrigationDocument.setModified();

	const auto result = irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().createFromStartTimeDto(startTimeDto);
	return std::make_pair(result.first, result.second->toString());
}
