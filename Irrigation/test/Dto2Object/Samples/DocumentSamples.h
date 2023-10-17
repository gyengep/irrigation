#pragma once
#include <memory>
#include <vector>
#include "DTO/DocumentDTO.h"
#include "Model/Impl/IrrigationDocumentImpl.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "ProgramContainerSamples.h"
#include "SampleBase.h"
#include <gmock/gmock.h>

using namespace testing;


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<IrrigationDocumentImpl, DocumentDTO> DocumentSample;

	class DocumentSampleList : public std::vector<DocumentSample> {
	public:
		DocumentSampleList() {

			emplace_back(
				std::make_shared<IrrigationDocumentImpl>(
					std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
					std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
					ProgramContainerSampleList().at(0).getContainerPtr(),
					nullptr,
					nullptr
				),
				DocumentDTO(std::list<ProgramDTO>(ProgramContainerSampleList().at(0).getDtoList()))
			);

			emplace_back(
				std::make_shared<IrrigationDocumentImpl>(
					std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
					std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
					ProgramContainerSampleList().at(1).getContainerPtr(),
					nullptr,
					nullptr
				),
				DocumentDTO(std::list<ProgramDTO>(ProgramContainerSampleList().at(1).getDtoList()))
			);

			emplace_back(
				std::make_shared<IrrigationDocumentImpl>(
					std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
					std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
					ProgramContainerSampleList().at(2).getContainerPtr(),
					nullptr,
					nullptr
				),
				DocumentDTO(std::list<ProgramDTO>(ProgramContainerSampleList().at(2).getDtoList()))
			);

			emplace_back(
				std::make_shared<IrrigationDocumentImpl>(
					std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
					std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
					ProgramContainerSampleList().at(3).getContainerPtr(),
					nullptr,
					nullptr
				),
				DocumentDTO(std::list<ProgramDTO>(ProgramContainerSampleList().at(3).getDtoList()))
			);
		}
	};
};
