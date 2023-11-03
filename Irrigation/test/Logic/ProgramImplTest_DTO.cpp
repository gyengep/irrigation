#include "ProgramImplTest.h"
#include "Exceptions/Exceptions.h"
#include "DtoSamples/SchedulersDtoSamples.h"
#include "DtoSamples/RunTimeDtoListSamples.h"
#include "DtoSamples/StartTimeDtoListSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const std::string ProgramImplDtoTest::originalName("program123456");
const std::string ProgramImplDtoTest::newName("98764_testProgram");

const SchedulersDto ProgramImplDtoTest::schedulersDtoSample(DtoSamples::schedulersDtoSample1);
const RunTimeDtoList ProgramImplDtoTest::runTimeDtoListSample(DtoSamples::runTimeDtoListSample1);
const StartTimeDtoList ProgramImplDtoTest::startTimeDtoListSample(DtoSamples::startTimeDtoListSample1);

///////////////////////////////////////////////////////////////////////////////

void ProgramImplDtoTest::SetUp() {
	ProgramImplTest::SetUp();

	program->setName(originalName);
	program->setAdjustment(originalAdjustment);
	program->setEnabled(originalEnabled);
	program->setSchedulerType(originalSchedulerType);

	ON_CALL(*mockSchedulerContainer, toSchedulersDto()).WillByDefault(Return(schedulersDtoSample));
	ON_CALL(*mockRunTimeContainer, toRunTimeDtoList()).WillByDefault(Return(runTimeDtoListSample));
	ON_CALL(*mockStartTimeContainer, toStartTimeDtoList()).WillByDefault(Return(startTimeDtoListSample));
}

void ProgramImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramImplDtoTest, toProgramDto) {
	const ProgramDto expectedProgramDto(
			originalEnabled,
			originalName,
			originalAdjustment,
			to_string(originalSchedulerType),
			SchedulersDto(schedulersDtoSample),
			RunTimeDtoList(runTimeDtoListSample),
			StartTimeDtoList(startTimeDtoListSample)
		);

	EXPECT_CALL(*mockSchedulerContainer, toSchedulersDto()).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(1);

	EXPECT_THAT(program->toProgramDto(), Eq(expectedProgramDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramImplDtoTest, updateFromProgramDto_empty) {
	ProgramDto programDto;

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_enabled) {
	ProgramDto programDto;
	programDto.setEnabled(newEnabled);

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(newEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_name) {
	ProgramDto programDto;
	programDto.setName(newName);

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(newName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_adjustament) {
	ProgramDto programDto;
	programDto.setAdjustment(newAdjustment);

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(newAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_schedulerType) {
	ProgramDto programDto;
	programDto.setSchedulerType(to_string(newSchedulerType));

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(newSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_schedulerContainer) {
	ProgramDto programDto;
	programDto.setSchedulers(SchedulersDto(schedulersDtoSample));

	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(schedulersDtoSample));

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_runTimeContainer) {
	ProgramDto programDto;
	programDto.setRunTimes(RunTimeDtoList(runTimeDtoListSample));

	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(runTimeDtoListSample));

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_partial_startTimeContainer) {
	ProgramDto programDto;
	programDto.setStartTimes(StartTimeDtoList(startTimeDtoListSample));

	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(startTimeDtoListSample));

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(originalEnabled));
	EXPECT_THAT(program->getName(), Eq(originalName));
	EXPECT_THAT(program->getAdjustment(), Eq(originalAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(originalSchedulerType));
}

TEST_F(ProgramImplDtoTest, updateFromProgramDto_all) {
	const ProgramDto programDto(
			newEnabled,
			newName,
			newAdjustment,
			to_string(newSchedulerType),
			SchedulersDto(schedulersDtoSample),
			RunTimeDtoList(runTimeDtoListSample),
			StartTimeDtoList(startTimeDtoListSample)
		);

	EXPECT_CALL(*mockSchedulerContainer, updateFromSchedulersDto(schedulersDtoSample));
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(runTimeDtoListSample));
	EXPECT_CALL(*mockStartTimeContainer, updateFromStartTimeDtoList(startTimeDtoListSample));

	program->updateFromProgramDto(programDto);

	EXPECT_THAT(program->isEnabled(), Eq(newEnabled));
	EXPECT_THAT(program->getName(), Eq(newName));
	EXPECT_THAT(program->getAdjustment(), Eq(newAdjustment));
	EXPECT_THAT(program->getSchedulerType(), Eq(newSchedulerType));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramImplDtoTest, updateFromProgramDto_IllegalArgumentException) {
	ProgramDto programDto;
	programDto.setSchedulerType("notExistinSchedulerType");

	EXPECT_THROW(program->updateFromProgramDto(programDto), IllegalArgumentException);
}
