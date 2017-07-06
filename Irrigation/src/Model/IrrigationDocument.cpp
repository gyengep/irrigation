#include "common.h"
#include "IrrigationDocument.h"

#include "Hardware/Valves.h"
#include "Logic/ContainerFactories.h"
#include "Logic/Program.h"
#include "Model/Application.h"


IrrigationDocument::IrrigationDocument() :
	programs(ProgramContainerFactoryHolder::create()),
	zones(MASTER_VALVE_ID, ZONE_VALVE_IDS)
{
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::load() {
	Program* program;

	program = new Program();
	program->setName("fulocsolas");
	program->getRunTimes().modify(0, 25);
	program->getRunTimes().modify(1, 35);
	program->getRunTimes().modify(2, 25);
	program->getStartTimes().insert(StartTime(4, 0));
	getPrograms().insert(program);

	program = new Program();
	program->setName("buxus");
	program->getRunTimes().modify(3, 20);
	program->getStartTimes().insert(StartTime(6, 0));
	getPrograms().insert(program);

	program = new Program();
	program->setName("paradicsom");
	program->getRunTimes().modify(4, 3);
	program->getStartTimes().insert(StartTime(8, 0));
	getPrograms().insert(program);
}
