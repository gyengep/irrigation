#include "TimerViewTest.h"
#include "Logic/ProgramContainer.h"

using namespace std;
using namespace testing;


const time_t TimerViewTest::rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
const RunTimeContainer TimerViewTest::runTimes1 { 110, 120, 130, 140, 150, 160 };
const RunTimeContainer TimerViewTest::runTimes2 { 210, 220, 230, 240, 250, 260 };


void TimerViewTest::SetUp() {
	wateringController.reset(new MockWateringController());
	ON_CALL(*wateringController, isWateringActive()).WillByDefault(Return(false));

	scheduler1.reset(new MockScheduler());
	ON_CALL(*scheduler1, getAdjustment()).WillByDefault(Return(adjustment1));

	program1.reset(new MockProgram());
	ON_CALL(*program1, isScheduled(_)).WillByDefault(Return(false));
	ON_CALL(*program1, getCurrentScheduler()).WillByDefault(ReturnRef(*scheduler1));
	program1->getRunTimes() = runTimes1;

	scheduler2.reset(new MockScheduler());
	ON_CALL(*scheduler2, getAdjustment()).WillByDefault(Return(adjustment2));

	program2.reset(new MockProgram());
	ON_CALL(*program2, isScheduled(_)).WillByDefault(Return(false));
	ON_CALL(*program2, getCurrentScheduler()).WillByDefault(ReturnRef(*scheduler2));
	program2->getRunTimes() = runTimes2;
}

void TimerViewTest::TearDown() {

}

void TimerViewTest::executeTest() {
	document = IrrigationDocument::Builder()
		.setWateringController(move(wateringController))
		.build();

	document->getPrograms().insert(IdType(), program1.release());
	document->getPrograms().insert(IdType(), program2.release());

	view.reset(new TimerView(*document));
	view->onTimer(rawTime);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerViewTest, notActiveNotScheduled) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*program1, isScheduled(rawTime)).Times(1);
	EXPECT_CALL(*program2, isScheduled(rawTime)).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawTime)).Times(1);

	executeTest();
}

TEST_F(TimerViewTest, notActiveScheduledFirst) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*program1, isScheduled(rawTime)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*program1, getCurrentScheduler()).Times(1);
	EXPECT_CALL(*scheduler1, getAdjustment()).Times(1);
	EXPECT_CALL(*wateringController, start(rawTime, runTimes1, adjustment1)).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawTime)).Times(1);

	executeTest();
}

TEST_F(TimerViewTest, notActiveScheduledSecond) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*program1, isScheduled(rawTime)).Times(1);
	EXPECT_CALL(*program2, isScheduled(rawTime)).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*program2, getCurrentScheduler()).Times(1);
	EXPECT_CALL(*scheduler2, getAdjustment()).Times(1);
	EXPECT_CALL(*wateringController, start(rawTime, runTimes2, adjustment2)).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawTime)).Times(1);

	executeTest();
}

TEST_F(TimerViewTest, active) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*wateringController, on1SecTimer(rawTime)).Times(1);

	executeTest();
}
