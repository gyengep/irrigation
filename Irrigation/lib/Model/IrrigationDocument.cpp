#include "IrrigationDocument.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"

using namespace std;


IrrigationDocument::IrrigationDocument()  {
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::lock() const {
	mutex.lock();
}

void IrrigationDocument::unlock() const {
	mutex.lock();
}

void IrrigationDocument::load() {
	lock_guard<std::mutex> lock(mutex);

	Program* program;

	program = new Program("fulocsolas");
	program->getRunTimes().at(0)->setValue(25);
	program->getRunTimes().at(1)->setValue(35);
	program->getRunTimes().at(2)->setValue(25);
	program->getStartTimes().insert(0, new StartTime(4, 0, 0));
	getPrograms().insert(0, program);

	program = new Program("buxus");
	program->getRunTimes().at(3)->setValue(20);
	program->getStartTimes().insert(1, new StartTime(6, 0, 0));
	getPrograms().insert(1, program);

	program = new Program("paradicsom");
	program->getRunTimes().at(4)->setValue(3);
	program->getStartTimes().insert(2, new StartTime(8, 0, 0));
	getPrograms().insert(2, program);
}

void IrrigationDocument::on1SecTimer(const time_t& rawTime) {
	lock_guard<std::mutex> lock(mutex);

	if (!wateringController.isWateringActive()) {
		for (auto it = programs.begin(); programs.end() != it; ++it) {
			const Program* program = it->second;
			if (program->isScheduled(rawTime)) {
				wateringController.start(rawTime, program->getRunTimes());
				break;
			}
		}
	}

	wateringController.on1SecTimer(rawTime);
}
