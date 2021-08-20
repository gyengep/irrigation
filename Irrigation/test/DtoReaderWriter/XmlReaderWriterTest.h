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

class RunTimeListReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};


class RunTimeListWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	RunTimeListWriterTest() : writer(false) {}

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

class StartTimeListReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class StartTimeListWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	StartTimeListWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class HotWeatherSchedulerWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	HotWeatherSchedulerWriterTest() : writer(false) {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class TemperatureDependentSchedulerWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	TemperatureDependentSchedulerWriterTest() : writer(false) {}

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

class ProgramListReaderTest : public ::testing::Test {
protected:

	XmlReader reader;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

class ProgramListWriterTest : public ::testing::Test {
protected:

	XmlWriter writer;

	ProgramListWriterTest() : writer(false) {}

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
