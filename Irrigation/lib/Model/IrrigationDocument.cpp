#include "IrrigationDocument.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"

using namespace std;


IrrigationDocument::IrrigationDocument() :
	IrrigationDocument(ZoneHandler::getInstancePtr())
{
}

IrrigationDocument::IrrigationDocument(shared_ptr<ZoneHandler> zoneHandler) :
	programs(new ProgramContainer()),
	wateringController(zoneHandler)
{
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::lock() const {
	mutex.lock();
}

void IrrigationDocument::unlock() const {
	mutex.unlock();
}

DocumentDTO IrrigationDocument::getDocumentDTO() const {
	lock_guard<std::mutex> lock(mutex);

	unique_ptr<list<ProgramDTO>> programDTOs(new list<ProgramDTO>());
	for (auto it = programs->begin(); it != programs->end(); ++it) {
		programDTOs->push_back(it->second->getProgramDTO().setId(it->first));
	}

	return DocumentDTO(programDTOs.release());
}

void IrrigationDocument::updateFromDTO(const DocumentDTO& documentDTO) {
	lock_guard<std::mutex> lock(mutex);

	if (documentDTO.hasPrograms()) {
		programs.reset(new ProgramContainer());

		for (const ProgramDTO& programDTO : documentDTO.getPrograms()) {
			unique_ptr<Program> program(new Program());
			program->updateFromDTO(programDTO);

			IdType id;
			if (programDTO.hasId()) {
				id = IdType(programDTO.getId());
			}

			programs->insert(id, program.release());
		}
	}
}
