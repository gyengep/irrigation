#include "TimerView.h"
#include "ChronoTools.h"
#include <iomanip>
#include <sstream>
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/Program.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"

using namespace std;
using namespace std::chrono;


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	period(seconds(1)),
	maxTardiness(seconds(1)),
	irrigationDocument(irrigationDocument),
	timer(*this, period)
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
		tm timeinfo = *localtime(&rawTime);
		for (auto& programAndIdPair : programs) {
			const IdType& idType = programAndIdPair.first;
			const Program& program = *programAndIdPair.second.get();
			if (program.isScheduled(timeinfo)) {
				LOGGER.debug("Program[%s] (%s) '%s' scheduler is scheduled",
						to_string(idType).c_str(),
						program.getName().c_str(),
						to_string(program.getSchedulerType()).c_str());
				wateringController.start(rawTime, program.getRunTimes(), program.getCurrentScheduler().getAdjustment());
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

	expectedSystemTime += period;
}

bool TimerView::checkSystemTime(const system_clock::time_point& expectedSystemTime) {
	const seconds actualDiff = duration_cast<seconds>(system_clock::now() - expectedSystemTime);

	if (abs(actualDiff) > seconds(1)) {
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

