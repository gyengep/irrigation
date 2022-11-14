#include "TimerView.h"
#include "Logger/Logger.h"
#include "Utils/FixedRateSchedulerRunnable.h"
#include "Utils/ReferenceRunnable.h"


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	period(std::chrono::seconds(1)),
	maxTardiness(std::chrono::seconds(1)),
	expectedLocalDateTime(DateTime::epoch()),
	lastRun(DateTime::epoch()),
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

	if (lastRun.getYears() != expectedLocalDateTime.getYears() ||
		lastRun.getMonths() != expectedLocalDateTime.getMonths() ||
		lastRun.getDays() != expectedLocalDateTime.getDays() ||
		lastRun.getHours() != expectedLocalDateTime.getHours() ||
		lastRun.getMinutes() != expectedLocalDateTime.getMinutes()
	)
	{
		try {
			std::lock_guard<IrrigationDocument> lock(irrigationDocument);
			irrigationDocument.startIfScheduled(expectedLocalDateTime);
		} catch (const std::exception& e) {
			LOGGER.warning("Start watering failed", e);
		}

		lastRun = expectedLocalDateTime;
	}

	expectedLocalDateTime = expectedLocalDateTime + period;
}

bool TimerView::checkExpectedDateTime(const LocalDateTime& expectedLocalDateTime) {
	const LocalDateTime now = LocalDateTime::now();
	const std::chrono::seconds actualDiff = (now - expectedLocalDateTime);

	if (actualDiff < std::chrono::seconds(-1) || std::chrono::seconds(1) < actualDiff) {

		LOGGER.warning("Time is changed! from %s to %s",
				expectedLocalDateTime.toString("%Y.%m.%d %H:%M:%S").c_str(),
				now.toString("%Y.%m.%d %H:%M:%S").c_str()
			);

		return false;
	}

	return true;
}

