#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockFileReader.h"
#include "Mocks/MockFileWriter.h"
#include "Persistence/JsonTemperatureDependentSchedulerRepository.h"


///////////////////////////////////////////////////////////////////////////////

class JsonTemperatureDependentSchedulerRepositoryTest : public ::testing::Test {
protected:

	std::shared_ptr<JsonTemperatureDependentSchedulerRepository> repository;
	std::shared_ptr<MockFileReader> mockFileReader;
	std::shared_ptr<MockFileWriterFactory> mockFileWriterFactory;

	virtual void SetUp();
    virtual void TearDown();
};
