#include "TimerView.h"
#include "Email/EmailHandler.h"
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"
#include <string>

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

	expectedSystemTime = system_clock::now() + period;

	timer.reset(new Timer(period, Timer::ScheduleType::FIXED_RATE, "TimerView"));
	timer->add(this);
	timer->start(Timer::Priority::HIGH);

	LOGGER.debug("TimerView initialized");
}

void TimerView::terminate() {
	LOGGER.debug("TimerView terminating...");

	timer->stop();
	timer.reset();

	LOGGER.debug("TimerView terminated");
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

void TimerView::onTimer(const time_t rawTime) {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	const ProgramContainer& programs = irrigationDocument.getPrograms();

	if (!irrigationDocument.getWateringController().isWateringActive()) {
		for (const auto& programAndIdPair : programs) {

			const IdType& idType = programAndIdPair.first;
			const shared_ptr<Program>& program = programAndIdPair.second;

			if (processProgramScheduled(idType, program, rawTime)) {
				break;
			}
		}
	}
}

bool TimerView::processProgramScheduled(const IdType& idType, const std::shared_ptr<Program>& program, const time_t rawTime) {
	try {
		const auto result = program->isScheduled(rawTime);
		if (result.first) {
			if (0 < result.second) {
				LOGGER.debug("Program[%s] (%s) '%s' scheduler is scheduled",
						to_string(idType).c_str(),
						program->getName().c_str(),
						to_string(program->getSchedulerType()).c_str());

				const EmailTopic topic = EmailTopic::WATERING_START;
				if (EMAIL.isTopicEnabled(topic)) {
					std::ostringstream oss;
					oss << "The " << program->getName() << " is scheduled by the " << to_string(program->getSchedulerType()) << " scheduler at " << toLocalTimeStr(rawTime, "%T") << std::endl;
					oss << "adjustment: "<< result.second << "%%" << std::endl;
					oss << "runTimes:   " << program->getRunTimes() << std::endl;
					EMAIL.send(topic, oss.str());
				}

				irrigationDocument.getWateringController().start(program->getRunTimes(), result.second);
			} else {
				const EmailTopic topic = EmailTopic::WATERING_SKIP;
				if (EMAIL.isTopicEnabled(topic)) {
					std::ostringstream oss;
					oss << "The " << program->getName() << " is skipped by the " << to_string(program->getSchedulerType()) << " scheduler at " << toLocalTimeStr(rawTime, "%T") << std::endl;
					EMAIL.send(topic, oss.str());
				}
			}

			irrigationDocument.saveState();
			return true;
		}
	}
	catch (const exception& e) {
		LOGGER.warning("Program checking failed", e);
	}

	return false;
}

bool TimerView::checkSystemTime(const system_clock::time_point& expectedSystemTime) {
	const seconds actualDiff = duration_cast<seconds>(system_clock::now() - expectedSystemTime);

	if (abs(actualDiff) > seconds(1)) {
		const time_t previousTime = system_clock::to_time_t(expectedSystemTime);
		const time_t currentTime = system_clock::to_time_t(system_clock::now());

		LOGGER.warning("Time is changed! from %s to %s",
				toLocalTimeStr(previousTime, "%Y.%m.%d %H:%M:%S").c_str(),
				toLocalTimeStr(currentTime, "%Y.%m.%d %H:%M:%S").c_str()
			);

		return false;
	}

	return true;
}

