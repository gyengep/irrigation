#include "IrrigationDocumentImplWateringTest.h"
#include "TestCommon/cout.h"
#include "Exceptions/Exceptions.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const LocalDateTime IrrigationDocumentImplWateringTest::localDateTime(2021, 11, 10, 22, 27, 30);

const IdType IrrigationDocumentImplWateringTest::programId1(15);
const IdType IrrigationDocumentImplWateringTest::programId2(25);

const std::string IrrigationDocumentImplWateringTest::name1("mockProgram1");
const std::string IrrigationDocumentImplWateringTest::name2("mockProgram2");

const DurationList IrrigationDocumentImplWateringTest::durationList1 {
	std::chrono::seconds(110),
	std::chrono::seconds(120),
	std::chrono::seconds(130),
	std::chrono::seconds(140),
	std::chrono::seconds(150),
	std::chrono::seconds(160)
};

const DurationList IrrigationDocumentImplWateringTest::durationList2 {
	std::chrono::seconds(210),
	std::chrono::seconds(220),
	std::chrono::seconds(230),
	std::chrono::seconds(240),
	std::chrono::seconds(250),
	std::chrono::seconds(260)
};

void IrrigationDocumentImplWateringTest::SetUp() {
	mockWateringController = std::make_shared<StrictMock<MockWateringController>>();

	mockRunTimeContainer1 = std::make_shared<StrictMock<MockRunTimeContainer>>();
	EXPECT_CALL(*mockRunTimeContainer1, toDurationList()).WillRepeatedly(Return(durationList1));

	mockRunTimeContainer2 = std::make_shared<StrictMock<MockRunTimeContainer>>();
	EXPECT_CALL(*mockRunTimeContainer2, toDurationList()).WillRepeatedly(Return(durationList2));

	mockProgram1 = std::make_shared<StrictMock<MockProgram>>();
	EXPECT_CALL(*mockProgram1, getRunTimeContainer()).WillRepeatedly(ReturnRef(*mockRunTimeContainer1));
	EXPECT_CALL(*mockProgram1, getAdjustment()).WillRepeatedly(Return(programAdjustment1));
	EXPECT_CALL(*mockProgram1, getSchedulerType()).WillRepeatedly(Return(SchedulerType::EVERY_DAY));
	EXPECT_CALL(*mockProgram1, getName()).WillRepeatedly(ReturnRef(name1));
	EXPECT_CALL(*mockProgram1, saveTo()).WillRepeatedly(Return(nlohmann::json()));

	mockProgram2 = std::make_shared<StrictMock<MockProgram>>();
	EXPECT_CALL(*mockProgram2, getRunTimeContainer()).WillRepeatedly(ReturnRef(*mockRunTimeContainer2));
	EXPECT_CALL(*mockProgram2, getAdjustment()).WillRepeatedly(Return(programAdjustment2));
	EXPECT_CALL(*mockProgram2, getSchedulerType()).WillRepeatedly(Return(SchedulerType::EVERY_DAY));
	EXPECT_CALL(*mockProgram2, getName()).WillRepeatedly(ReturnRef(name2));
	EXPECT_CALL(*mockProgram2, saveTo()).WillRepeatedly(Return(nlohmann::json()));

	programList.push_back(ProgramContainer::value_type{ programId1, mockProgram1 });
	programList.push_back(ProgramContainer::value_type{ programId2, mockProgram2 });

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	EXPECT_CALL(*mockProgramContainer, at(programId1)).WillRepeatedly(Return(mockProgram1));
	EXPECT_CALL(*mockProgramContainer, at(programId2)).WillRepeatedly(Return(mockProgram2));
	EXPECT_CALL(*mockProgramContainer, begin()).WillRepeatedly(Return(programList.begin()));
	EXPECT_CALL(*mockProgramContainer, end()).WillRepeatedly(Return(programList.end()));

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
			std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
			mockProgramContainer,
			mockWateringController,
			nullptr
		);
}

void IrrigationDocumentImplWateringTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplWateringTest, startCustom) {

	EXPECT_CALL(*mockWateringController, start(durationList1.adjust(adjustment1))).
			Times(1);

	irrigationDocument->startCustom(durationList1, adjustment1);
}

TEST_F(IrrigationDocumentImplWateringTest, startProgram) {

	EXPECT_CALL(*mockWateringController, start(durationList1.adjust(adjustment1))).
			Times(1);

	irrigationDocument->startProgram(programId1, adjustment1);
}

TEST_F(IrrigationDocumentImplWateringTest, startProgram_notExiting) {
	const IdType notExistingProgramId(150);

	EXPECT_CALL(*mockProgramContainer, at(notExistingProgramId)).
			Times(1).
			WillOnce(Throw(NoSuchElementException("")));

	EXPECT_THROW(irrigationDocument->startProgram(notExistingProgramId, 80), NoSuchElementException);
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_alreadyActive) {

	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(true));

	irrigationDocument->startIfScheduled(LocalDateTime::now());
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_notActive_notScheduled) {

	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(false));

	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));

	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_notActive_scheduledFirst) {

	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(false));

	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment1)))));

	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).
			Times(0);

	EXPECT_CALL(*mockWateringController, start(durationList1.adjust(adjustment1).adjust(programAdjustment1))).
			Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_notActive_scheduledSecond) {

	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(false));

	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));

	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment2)))));

	EXPECT_CALL(*mockWateringController, start(durationList2.adjust(adjustment2).adjust(programAdjustment2))).
			Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_notActive_skippedFirst) {

	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(false));

	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));

	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment2)))));

	EXPECT_CALL(*mockWateringController, start(durationList2.adjust(adjustment2).adjust(programAdjustment2))).
			Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIfScheduled_notActive_skippedBoth) {
	EXPECT_CALL(*mockWateringController, isWateringActive()).
			Times(1).
			WillOnce(Return(false));

	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));

	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).
			Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, stop) {
	EXPECT_CALL(*mockWateringController, stop()).
			Times(1);

	irrigationDocument->stop();
}
