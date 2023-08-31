#include "IrrigationDocumentImplWateringTest.h"
#include "TestCommon/cout.h"
#include "Logic/ProgramContainerImpl.h"
#include "Logic/RunTimeImpl.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const LocalDateTime IrrigationDocumentImplWateringTest::localDateTime(2021, 11, 10, 22, 27, 30);

const IdType IrrigationDocumentImplWateringTest::id1(15);
const IdType IrrigationDocumentImplWateringTest::id2(25);

const std::string IrrigationDocumentImplWateringTest::name1("mockProgram1");
const std::string IrrigationDocumentImplWateringTest::name2("mockProgram2");

void IrrigationDocumentImplWateringTest::SetUp() {
	mockWateringController = std::make_shared<StrictMock<MockWateringController>>();


	mockProgram1 = std::make_shared<StrictMock<MockProgram>>();

	runTimeContainer1 = std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr>({
			std::make_shared<RunTimeImpl>(110),
			std::make_shared<RunTimeImpl>(120),
			std::make_shared<RunTimeImpl>(130),
			std::make_shared<RunTimeImpl>(140),
			std::make_shared<RunTimeImpl>(150),
			std::make_shared<RunTimeImpl>(160)
		}));

	EXPECT_CALL(*mockProgram1, getRunTimeContainer()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*runTimeContainer1));
	EXPECT_CALL(*mockProgram1, getAdjustment()).Times(AnyNumber()).WillRepeatedly(Return(programAdjustment1));
	EXPECT_CALL(*mockProgram1, getSchedulerType()).Times(AnyNumber()).WillRepeatedly(Return(SchedulerType::EVERY_DAY));
	EXPECT_CALL(*mockProgram1, getName()).Times(AnyNumber()).WillRepeatedly(ReturnRef(name1));
	EXPECT_CALL(*mockProgram1, saveTo()).Times(AnyNumber());


	mockProgram2 = std::make_shared<StrictMock<MockProgram>>();

	runTimeContainer2 = std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr>({
			std::make_shared<RunTimeImpl>(210),
			std::make_shared<RunTimeImpl>(220),
			std::make_shared<RunTimeImpl>(230),
			std::make_shared<RunTimeImpl>(240),
			std::make_shared<RunTimeImpl>(250),
			std::make_shared<RunTimeImpl>(260)
		}));

	EXPECT_CALL(*mockProgram2, getRunTimeContainer()).Times(AnyNumber()).WillRepeatedly(ReturnRef(*runTimeContainer2));
	EXPECT_CALL(*mockProgram2, getAdjustment()).Times(AnyNumber()).WillRepeatedly(Return(programAdjustment2));
	EXPECT_CALL(*mockProgram2, getSchedulerType()).Times(AnyNumber()).WillRepeatedly(Return(SchedulerType::EVERY_DAY));
	EXPECT_CALL(*mockProgram2, getName()).Times(AnyNumber()).WillRepeatedly(ReturnRef(name2));
	EXPECT_CALL(*mockProgram2, saveTo()).Times(AnyNumber());


	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			std::make_shared<ProgramContainerImpl>(
				std::initializer_list<ProgramContainer::value_type> {
						{ id1, mockProgram1 },
						{ id2, mockProgram2 }
					}),
			mockWateringController,
			nullptr
		);
}

void IrrigationDocumentImplWateringTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplWateringTest, startCustom) {
	const unsigned adjustment = 90;

	const DurationList durations {
		std::chrono::seconds(100),
		std::chrono::seconds(200),
		std::chrono::seconds(300),
		std::chrono::seconds(400),
		std::chrono::seconds(500),
		std::chrono::seconds(600),
	};

	EXPECT_CALL(*mockWateringController, start(durations.adjust(adjustment))).Times(1);

	irrigationDocument->startCustom(durations, adjustment);
}

TEST_F(IrrigationDocumentImplWateringTest, startProgram) {
	const unsigned adjustment = 90;
	const DurationList durations = runTimeContainer1->toDurationList();

	EXPECT_CALL(*mockWateringController, start(durations.adjust(adjustment))).Times(1);

	irrigationDocument->startProgram(id1, adjustment);
}


TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_alreadyActive) {
	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).WillOnce(Return(true));

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_notActiveNotScheduled) {
	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).
			WillOnce(Return(false));
	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));
	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_notActiveScheduledFirst) {
	const unsigned adjustment = 90;

	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).
			WillOnce(Return(false));
	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment)))));
	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).Times(0);

	EXPECT_CALL(*mockWateringController, start(mockProgram1->getRunTimeContainer().toDurationList().adjust(programAdjustment1).adjust(adjustment))).Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_notActiveScheduledSecond) {
	const unsigned adjustment = 80;

	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).
			WillOnce(Return(false));
	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>())));
	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment)))));

	EXPECT_CALL(*mockWateringController, start(mockProgram2->getRunTimeContainer().toDurationList().adjust(programAdjustment2).adjust(adjustment))).Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_notActiveSkippedFirst) {
	const unsigned adjustment = 80;

	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).
			WillOnce(Return(false));
	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));
	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment)))));

	EXPECT_CALL(*mockWateringController, start(mockProgram2->getRunTimeContainer().toDurationList().adjust(programAdjustment2).adjust(adjustment))).Times(1);

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, startIdScheduled_notActiveSkippedBoth) {
	EXPECT_CALL(*mockWateringController, isWateringActive()).Times(1).
			WillOnce(Return(false));
	EXPECT_CALL(*mockProgram1, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));
	EXPECT_CALL(*mockProgram2, isScheduled(localDateTime)).Times(1).
			WillOnce(Return(ByMove(std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false)))));

	irrigationDocument->startIfScheduled(localDateTime);
}

TEST_F(IrrigationDocumentImplWateringTest, stop) {
	EXPECT_CALL(*mockWateringController, stop()).Times(1);

	irrigationDocument->stop();
}

