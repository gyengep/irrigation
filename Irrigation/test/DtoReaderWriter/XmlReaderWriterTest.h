#pragma once
#include <gmock/gmock.h>
#include "DTO/DocumentDTO.h"
#include "DTO/ProgramDTO.h"
#include "DTO/RunTimeDTO.h"
#include "DTO/StartTimeDTO.h"
#include "DTO/WeeklySchedulerDTO.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"


std::string remove_xml_tag(const std::string& text);

///////////////////////////////////////////////////////////////////////////////

class RunTimeReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};


class RunTimeWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	RunTimeWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class StartTimeWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	StartTimeWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class PeriodicSchedulerReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class PeriodicSchedulerWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	PeriodicSchedulerWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class WeeklySchedulerReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class WeeklySchedulerWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	WeeklySchedulerWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class ProgramReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class ProgramWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	ProgramWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class DocumentReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class DocumentWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	DocumentWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////
