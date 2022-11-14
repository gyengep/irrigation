#include "TimerView.h"
#include "Logger/Logger.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/ReferenceRunnable.h"


TimerView::TimerView(IrrigationDocument& irrigationDocument) :
	View(irrigationDocument),
	period(std::chrono::milliseconds(500)),
	lastRun(DateTime::epoch()),
	irrigationDocument(irrigationDocument)
{
}

TimerView::~TimerView() {
}

void TimerView::initialize() {
	LOGGER.debug("TimerView initializing...");

	auto referenceRunnbale = std::make_shared<ReferenceRunnable>(*this);
	auto fixedDelaySchedulerRunnable = std::make_shared<FixedDelaySchedulerRunnable>(
			referenceRunnbale,
			period,
			period,
			"TimerView"
		);

	timerThread = std::unique_ptr<Thread>(new Thread(fixedDelaySchedulerRunnable, "TimerView"));
	timerThread->start();

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

	const LocalDateTime now = LocalDateTime::now();

	if (lastRun.getYears() != now.getYears() ||
		lastRun.getMonths() != now.getMonths() ||
		lastRun.getDays() != now.getDays() ||
		lastRun.getHours() != now.getHours() ||
		lastRun.getMinutes() != now.getMinutes()
	)
	{
		try {
			std::lock_guard<IrrigationDocument> lock(irrigationDocument);
			irrigationDocument.startIfScheduled(now);
		} catch (const std::exception& e) {
			LOGGER.warning("Start watering failed", e);
		}

		lastRun = now;
	}
}

