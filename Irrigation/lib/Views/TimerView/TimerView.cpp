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
	expectedLocalDateTime(DateTime::epoch()),
	irrigationDocument(irrigationDocument)
{
}

TimerView::~TimerView() {
}

void TimerView::initialize() {
	LOGGER.debug("TimerView initializing...");

	expectedLocalDateTime = DateTime::now() + period;

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

	if (!checkExpectedDateTime(expectedLocalDateTime)) {
		expectedLocalDateTime = DateTime::now();
	}

	checkProgramScheduled(expectedLocalDateTime);
	expectedLocalDateTime = expectedLocalDateTime + period;
}

void TimerView::checkProgramScheduled(const LocalDateTime& localDateTime) {
	lock_guard<IrrigationDocument> lock(irrigationDocument);

	if (!irrigationDocument.getWateringController().isWateringActive()) {
		const ProgramContainer& programs = irrigationDocument.getPrograms();

		for (const auto& programAndIdPair : programs) {

			const auto& id = programAndIdPair.first;
			const auto& program = programAndIdPair.second;

			if (processProgramScheduled(id, program, localDateTime)) {
				break;
			}
		}
	}
}

bool TimerView::processProgramScheduled(const IdType& idType, const std::shared_ptr<Program>& program, const LocalDateTime& localDateTime) {
	try {
		const auto scheduledResult = program->isScheduled(localDateTime);

		if (scheduledResult->isScheduled()) {
			if (0 < scheduledResult->getAdjustment()) {
				LOGGER.debug("Program[%s] (%s) '%s' scheduler is scheduled",
						idType.toString().c_str(),
						program->getName().c_str(),
						to_string(program->getSchedulerType()).c_str());

				const EmailTopic topic = EmailTopic::WATERING_START;
				if (EMAIL.isTopicEnabled(topic)) {
					std::ostringstream oss;
					oss << "The " << program->getName() << " is scheduled by the " << to_string(program->getSchedulerType()) << " scheduler at " << localDateTime.toString("%T") << std::endl;
					oss << "adjustment: "<< scheduledResult->getAdjustment() << "%" << std::endl;
					oss << "runTimes:   " << program->getRunTimeContainer() << std::endl;
					EMAIL.send(topic, oss.str());
				}

				irrigationDocument.getWateringController().start(program->getRunTimeContainer().toDurations(), scheduledResult->getAdjustment());
			} else {
				const EmailTopic topic = EmailTopic::WATERING_SKIP;
				if (EMAIL.isTopicEnabled(topic)) {
					std::ostringstream oss;
					oss << "The " << program->getName() << " is skipped by the " << to_string(program->getSchedulerType()) << " scheduler at " << localDateTime.toString("%T") << std::endl;
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

bool TimerView::checkExpectedDateTime(const LocalDateTime& expectedLocalDateTime) {
	const LocalDateTime now = LocalDateTime::now();
	const seconds actualDiff = (now - expectedLocalDateTime);

	if (actualDiff > seconds(1) || actualDiff < seconds(-1)) {

		LOGGER.warning("Time is changed! from %s to %s",
				expectedLocalDateTime.toString("%Y.%m.%d %H:%M:%S").c_str(),
				now.toString("%Y.%m.%d %H:%M:%S").c_str()
			);

		return false;
	}

	return true;
}

