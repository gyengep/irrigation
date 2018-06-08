#pragma once
#include <gmock/gmock.h>
#include "ReaderWriter/DocumentDTO.h"
#include "ReaderWriter/ProgramDTO.h"
#include "ReaderWriter/RunTimeDTO.h"
#include "ReaderWriter/StartTimeDTO.h"
#include "ReaderWriter/SchedulersDTO.h"
#include "ReaderWriter/XmlReader.h"
#include "ReaderWriter/XmlWriter.h"



class RunTimeReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;
	RunTimeDTO runTime;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class StartTimeReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;
	StartTimeDTO startTime;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class SpecifiedSchedulerReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;
	SpecifiedSchedulerDTO scheduler;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class ProgramReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;
	ProgramDTO program;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class DocumentReaderWriterTest : public ::testing::Test {
protected:

	XmlReader reader;
	XmlWriter writer;
	DocumentDTO document;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

