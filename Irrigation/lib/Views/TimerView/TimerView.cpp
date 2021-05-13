#include "TimerView.h"
#include "Email/EmailHandler.h"
#include "Logger/Logger.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Utils/FixedRateSchedulerRunnable.h"
#include "Utils/ReferenceRunnable.h"
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

	auto referenceRunnbale = std::make_shared<ReferenceRunnable>(*this);
	auto fixedRateSchedulerRunnable = std::make_shared<FixedRateSchedulerRunnable>(
			referenceRunnbale,
			period,
			"TimerView"
		);

	timerThread = std::unique_ptr<Thread>(new Thread(fixedRateSchedulerRunnable, "TimerView"));
	timerThread->start(Thread::Priority::HIGH);

	LOGGER.debug("TimerView initialized");
}

void TimerView::terminate() {
	LOGGER.debug("TimerView terminating...");

	timerThread->stop();
	timerThread.reset();

	LOGGER.debug("TimerView terminated");
}

void TimerView::run() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("TimerView::run()");
#endif

	if (!checkSystemTime(expectedSystemTime)) {
		expectedSystemTime = system_clock::now();
	}

	checkProgramScheduled(system_clock::to_time_t(expectedSystemTime));
	expectedSystemTime += period;
}

void TimerView::checkProgramScheduled(const time_t rawTime) {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	if (!irrigationDocument.getWateringController().isWateringActive()) {
		const ProgramContainer& programs = irrigationDocument.getPrograms();

		for (const auto& programAndIdPair : programs) {

			const auto& id = programAndIdPair.first;
			const auto& program = programAndIdPair.second;

			if (processProgramScheduled(id, program, rawTime)) {
				break;
			}
		}
	}
}

bool TimerView::processProgramScheduled(const IdType& idType, const std::shared_ptr<Program>& program, const time_t rawTime) {
	try {
		const auto scheduledResult = program->isScheduled(rawTime);

		if (scheduledResult->isScheduled()) {
			if (0 < scheduledResult->getAdjustment()) {
				LOGGER.debug("Program[%s] (%s) '%s' scheduler is scheduled",
						to_string(idType).c_str(),
						program->getName().c_str(),
						to_string(program->getSchedulerType()).c_str());

				const EmailTopic topic = EmailTopic::WATERING_START;
				if (EMAIL.isTopicEnabled(topic)) {
					std::ostringstream oss;
					oss << "The " << program->getName() << " is scheduled by the " << to_string(program->getSchedulerType()) << " scheduler at " << toLocalTimeStr(rawTime, "%T") << std::endl;
					oss << "adjustment: "<< scheduledResult->getAdjustment() << "%" << std::endl;
					oss << "runTimes:   " << program->getRunTimes() << std::endl;
					EMAIL.send(topic, oss.str());
				}

				irrigationDocument.getWateringController().start(program->getRunTimes(), scheduledResult->getAdjustment());
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

