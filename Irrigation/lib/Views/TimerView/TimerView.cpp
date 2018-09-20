#include "TimerView.h"
#include "ChronoTools.h"
#include <sstream>
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"

using namespace std;


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	irrigationDocument(irrigationDocument),
	timer(*this, chrono::seconds(1))
{
}

TimerView::~TimerView() {
}

void TimerView::initialize() {
	LOGGER.debug("TimerView initializing...");
	timer.start();
	LOGGER.debug("TimerView initialized");
}

void TimerView::terminate() {
	LOGGER.debug("TimerView terminating...");
	timer.stop();
	LOGGER.debug("TimerView terminated");
}

void TimerView::onTimer(time_t rawTime) {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	WateringController& wateringController = irrigationDocument.getWateringController();
	ProgramContainer& programs = irrigationDocument.getPrograms();

	if (!wateringController.isWateringActive()) {
		for (auto it = programs.begin(); programs.end() != it; ++it) {
			const IdType& idType = it->first;
			const Program* program = it->second.get();
			if (program->isScheduled(rawTime)) {
				LOGGER.debug("Program[%s] (%s) is scheduled",
						to_string(idType).c_str(),
						program->getName().c_str());
				wateringController.start(rawTime, program->getRunTimes(), program->getCurrentScheduler().getAdjustment());
				break;
			}
		}
	}

	wateringController.on1SecTimer(rawTime);
}
