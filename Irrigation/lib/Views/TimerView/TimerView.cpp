#include "TimerView.h"
#include "ChronoTools.h"
#include <iomanip>
#include <sstream>
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"

using namespace std;
using namespace std::chrono;


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	deltaT(seconds(1)),
	irrigationDocument(irrigationDocument),
	timer(*this, deltaT)
{
}

TimerView::~TimerView() {
}

void TimerView::initialize() {
	LOGGER.debug("TimerView initializing...");

	expectedSystemTime = system_clock::now();

	timer.start();
	LOGGER.debug("TimerView initialized");
}

void TimerView::terminate() {
	LOGGER.debug("TimerView terminating...");
	timer.stop();
	LOGGER.debug("TimerView terminated");
}

void TimerView::onTimer(const time_t rawTime) {
	WateringController& wateringController = irrigationDocument.getWateringController();
	const ProgramContainer& programs = irrigationDocument.getPrograms();

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

void TimerView::onTimer() {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	if (!checkSystemTime(expectedSystemTime)) {
		expectedSystemTime = system_clock::now();
	}

	onTimer(system_clock::to_time_t(expectedSystemTime));

	expectedSystemTime += deltaT;
}

bool TimerView::checkSystemTime(const system_clock::time_point& expectedSystemTime) {
	const system_clock::time_point currentTime = system_clock::now();
	const seconds actualDiff = duration_cast<seconds>(currentTime - expectedSystemTime);

	if (actualDiff < seconds(-1) || seconds(1) < actualDiff) {
		const time_t previousTime = system_clock::to_time_t(expectedSystemTime);
		const time_t currentTime = system_clock::to_time_t(system_clock::now());

		ostringstream o;
		o << "Time is changed! ";
		o << "from " << put_time(localtime(&previousTime), "%Y.%m.%d %H:%M:%S") << " ";
		o << "to " << put_time(localtime(&currentTime), "%Y.%m.%d %H:%M:%S");

		LOGGER.warning(o.str().c_str());
		return false;
	}

	return true;
}

