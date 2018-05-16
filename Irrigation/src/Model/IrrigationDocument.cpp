#include "IrrigationDocument.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"



IrrigationDocument::IrrigationDocument()  {
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::load() {
	Program* program;

	program = new Program();
	program->setName("fulocsolas");
	program->getRunTimes().at(0)->setValue(25);
	program->getRunTimes().at(1)->setValue(35);
	program->getRunTimes().at(2)->setValue(25);
	program->getStartTimes().insert(std::make_pair(0, new StartTime(4, 0, 0)));
	getPrograms().insert(program);

	program = new Program();
	program->setName("buxus");
	program->getRunTimes().at(3)->setValue(20);
	program->getStartTimes().insert(std::make_pair(1, new StartTime(6, 0, 0)));
	getPrograms().insert(program);

	program = new Program();
	program->setName("paradicsom");
	program->getRunTimes().at(4)->setValue(3);
	program->getStartTimes().insert(std::make_pair(2, new StartTime(8, 0, 0)));
	getPrograms().insert(program);
}
