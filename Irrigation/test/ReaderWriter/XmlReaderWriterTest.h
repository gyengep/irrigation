#pragma once
#include <ReaderWriter/XmlReader.h>

#include <gmock/gmock.h>
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/SchedulersDTO.h"
#include "ReaderWriter/XmlWriter.h"



class RunTimeReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class StartTimeReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class SpecifiedSchedulerReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class ProgramReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class DocumentReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

