#pragma once
#include <memory>
#include "Model/IrrigationDocument.h"
#include "ProgramListSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<IrrigationDocument, DocumentDTO> DocumentSample;


	class DocumentSample1 : public DocumentSample {
	public:
		DocumentSample1() : DocumentSample(
			IrrigationDocument::Builder().setProgramContainer(ProgramListSample1().getContainerPtr()).build(),
			DocumentDTO(std::list<ProgramDTO>(ProgramListSample1().getDtoList()))
		) {}
	};

	class DocumentSample2 : public DocumentSample {
	public:
		DocumentSample2() : DocumentSample(
			IrrigationDocument::Builder().setProgramContainer(ProgramListSample2().getContainerPtr()).build(),
			DocumentDTO(std::list<ProgramDTO>(ProgramListSample2().getDtoList()))
		) {}
	};

	class DocumentSample3 : public DocumentSample {
	public:
		DocumentSample3() : DocumentSample(
			IrrigationDocument::Builder().setProgramContainer(ProgramListSample3().getContainerPtr()).build(),
			DocumentDTO(std::list<ProgramDTO>(ProgramListSample3().getDtoList()))
		) {}
	};

	class DocumentSample4 : public DocumentSample {
	public:
		DocumentSample4() : DocumentSample(
			IrrigationDocument::Builder().setProgramContainer(ProgramListSample4().getContainerPtr()).build(),
			DocumentDTO(std::list<ProgramDTO>(ProgramListSample4().getDtoList()))
		) {}
	};
};
