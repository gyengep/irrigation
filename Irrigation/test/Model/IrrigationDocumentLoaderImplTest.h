#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Mocks/MockDtoReader.h"
#include "Mocks/MockFileReader.h"
#include "Model/Impl/IrrigationDocumentLoaderImpl.h"



class IrrigationDocumentLoaderImplTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocumentLoaderImpl> irrigationDocumentLoaderImpl;
	std::shared_ptr<MockDtoReader> mockDtoReader;
	std::shared_ptr<MockFileReader> mockFileReader;

    virtual void SetUp();
    virtual void TearDown();
};

