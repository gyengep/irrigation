#include "TimerView.h"
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/Program.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Utils/ChronoTools.h"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	period(seconds(1)),
	maxTardiness(seconds(1)),
	irrigationDocument(irrigationDocument)
{
}

TimerView::~TimerView() {
}

void TimerView::initialize() {
	LOGGER.debug("TimerView initializing...");

	timer.reset(new Timer(period, Timer::ScheduleType::FIXED_RATE));
	timer->add(this);
	expectedSystemTime = system_clock::now() + period;
	timer->start(Timer::Priority::HIGH);

	LOGGER.debug("TimerView initialized");
}

void TimerView::terminate() {
	LOGGER.debug("TimerView terminating...");

	timer->stop();
	timer.reset();

	LOGGER.debug("TimerView terminated");
}

void TimerView::onTimer(const time_t rawTime) {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	WateringController& wateringController = irrigationDocument.getWateringController();
	const ProgramContainer& programs = irrigationDocument.getPrograms();

	if (!wateringController.isWateringActive()) {
		const tm timeinfo = *localtime(&rawTime);
		for (auto& programAndIdPair : programs) {
			const IdType& idType = programAndIdPair.first;
			const shared_ptr<Program> program = programAndIdPair.second;
			if (program->isScheduled(timeinfo)) {
				LOGGER.debug("Program[%s] (%s) '%s' scheduler is scheduled",
						to_string(idType).c_str(),
						program->getName().c_str(),
						to_string(program->getSchedulerType()).c_str());
				wateringController.start(rawTime, program->getRunTimes(), program->getAdjustment());
				break;
			}
		}
	}

	wateringController.on1SecTimer(rawTime);
}

void TimerView::onTimer() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("TimerView::onTimer()");
#endif

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

