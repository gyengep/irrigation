#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Mocks/MockDtoWriter.h"
#include "Mocks/MockFileWriter.h"
#include "Model/IrrigationDocumentSaverImpl.h"



class IrrigationDocumentSaverImplTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocumentSaverImpl> irrigationDocumentSaverImpl;
	std::shared_ptr<MockDtoWriter> mockDtoWriter;
	std::shared_ptr<MockFileWriter> mockFileWriter;
	std::shared_ptr<MockFileWriterFactory> mockFileWriterFactory;

    virtual void SetUp();
    virtual void TearDown();
};

