#include "RestView.h"
#include "Model/IrrigationDocument.h"
#include "Logic/ProgramContainer.h"
#include "Logic/StartTimeContainer.h"
#include <mutex>


void RestView::deleteProgram(IrrigationDocument& irrigationDocument, const IdType& programId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	irrigationDocument.setModified();
	irrigationDocument.getProgramContainer().erase(programId);
}

void RestView::deleteStartTime(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId) {
	std::unique_lock<IrrigationDocument> lock(irrigationDocument);
	irrigationDocument.setModified();
	irrigationDocument.getProgramContainer().at(programId)->getStartTimeContainer().erase(startTimeId);
}
