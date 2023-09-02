#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Mocks/MockDtoWriter.h"
#include "Mocks/MockFileWriter.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/DocumentSaver.h"

class IrrigationDocument;


class MockDtoWriterFactory : public DocumentSaver::DtoWriterFactory {
public:
	const std::shared_ptr<MockDtoWriter> mockDtoWriter;

	MockDtoWriterFactory(std::shared_ptr<MockDtoWriter> mockDtoWriter) : mockDtoWriter(mockDtoWriter) {}
	virtual ~MockDtoWriterFactory() = default;
	virtual std::shared_ptr<DtoWriter> create() override {
		return mockDtoWriter;
	}
};

class DocumentSaverTest : public ::testing::Test {
protected:

	std::shared_ptr<DocumentSaver> documentSaver;
	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockDtoWriterFactory> mockDtoWriterFactory;
	std::shared_ptr<MockFileWriter> mockFileWriter;
	std::shared_ptr<MockFileWriterFactory> mockFileWriterFactory;
	std::shared_ptr<IrrigationDocument> irrigationDocument;

    virtual void SetUp();
    virtual void TearDown();
};

