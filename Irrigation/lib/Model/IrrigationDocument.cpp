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

void IrrigationDocument::on1SecTimer(const time_t& rawTime) {
	lock_guard<std::mutex> lock(mutex);

	if (!wateringController.isWateringActive()) {
		for (auto it = programs->begin(); programs->end() != it; ++it) {
			const Program* program = it->second;
			if (program->isScheduled(rawTime)) {
				wateringController.start(rawTime, program->getRunTimes());
				break;
			}
		}
	}

	wateringController.on1SecTimer(rawTime);
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
			if (!programDTO.hasId()) {
				throw logic_error("IrrigationDocument::updateFromDTO(): !program.hasId()");
			}

			ProgramContainer::value_type& idAndProgramPair = programs->insert(programDTO.getId(), new Program());
			idAndProgramPair.second->updateFromDTO(programDTO);
		}
	}
}
