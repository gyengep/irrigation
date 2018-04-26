#pragma once

#include <memory>

#include "Model/IrrigationDocument.h"
#include "Logic/Containers.h"
#include "Views/CommandLineView/Commands.h"


class CommandLineView;

namespace CommandLine {

template<typename CommandType>
class CommandsTest : public ::testing::Test {
protected:

	std::unique_ptr<IrrigationDocument> document;
	std::unique_ptr<CommandType> command;

    virtual void SetUp() {
    	document.reset(new IrrigationDocument());
    	command.reset(new CommandType(document.get()));
    }

    virtual void TearDown() {
    }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ListProgramTest : public CommandsTest<ProgramList> {
};

class ShowProgramTest : public CommandsTest<ProgramShow> {
};

class AddProgramTest : public CommandsTest<ProgramAdd> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class DelProgramTest : public CommandsTest<ProgramDelete> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class RenProgramTest : public CommandsTest<ProgramRename> {
};

class MoveProgramTest : public CommandsTest<ProgramMove> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ListStartTimeTest : public CommandsTest<StartTimeList> {
};

class AddStartTimeTest : public CommandsTest<StartTimeAdd> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class DelStartTimeTest : public CommandsTest<StartTimeDelete> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class SetStartTimeTest : public CommandsTest<StartTimeSet> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ListRunTimeTest : public CommandsTest<RunTimeList> {
};

class SetRunTimeTest : public CommandsTest<RunTimeSet> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ResetValvesTest : public CommandsTest<ResetValves> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class ValveTest : public CommandsTest<Valve> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class ZoneTest : public CommandsTest<Zone> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ExitTest : public CommandsTest<Exit> {
protected:

    virtual void SetUp();
    virtual void TearDown();
};

class HelpTest : public ::testing::Test {
protected:

	std::unique_ptr<CommandLineView> view;
	std::unique_ptr<Help> command;

    virtual void SetUp();
    virtual void TearDown();
};


}
