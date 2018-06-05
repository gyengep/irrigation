#pragma once
#include <gmock/gmock.h>
#include <list>
#include "ReaderWriter/XmlWriter.h"


class XmlWriterTest : public ::testing::Test {
protected:

	SpecifiedSchedulerDTO* createSpecifiedSchedulerDTO(const char* value1 = nullptr, const char* value2 = nullptr, const char* value3 = nullptr);
	std::list<RunTimeDTO>* createRunTimeDTOList(const char* value1 = nullptr, const char* value2 = nullptr, const char* value3 = nullptr);
	std::list<StartTimeDTO>* createStartTimeDTOList(const char* value1 = nullptr, const char* value2 = nullptr, const char* value3 = nullptr);

	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};
