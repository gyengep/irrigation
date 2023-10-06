#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockFileReader.h"
#include "Mocks/MockFileWriter.h"
#include "Persistence/JsonHotWeatherSchedulerRepository.h"


///////////////////////////////////////////////////////////////////////////////

class JsonHotWeatherSchedulerRepositoryTest : public ::testing::Test {
protected:

	std::shared_ptr<JsonHotWeatherSchedulerRepository> repository;
	std::shared_ptr<MockFileReader> mockFileReader;
	std::shared_ptr<MockFileWriterFactory> mockFileWriterFactory;

	virtual void SetUp();
    virtual void TearDown();
};
