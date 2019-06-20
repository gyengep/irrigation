#include "TimerViewTest.h"
#include "Logic/ProgramContainer.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const time_t TimerViewTest::rawtime = time(nullptr);
const tm TimerViewTest::timeinfo = *localtime(&TimerViewTest::rawtime);
const RunTimeContainer TimerViewTest::runTimes1 { 110, 120, 130, 140, 150, 160 };
const RunTimeContainer TimerViewTest::runTimes2 { 210, 220, 230, 240, 250, 260 };


void TimerViewTest::SetUp() {
	wateringController.reset(new MockWateringController());
	scheduler1.reset(new MockScheduler());
	scheduler2.reset(new MockScheduler());
	program1.reset(new MockProgram());
	program2.reset(new MockProgram());
	program1->setRunTimes(shared_ptr<RunTimeContainer>(new RunTimeContainer(runTimes1)));
	program2->setRunTimes(shared_ptr<RunTimeContainer>(new RunTimeContainer(runTimes2)));

	ON_CALL(*wateringController, isWateringActive()).WillByDefault(Return(false));
	ON_CALL(*program1, isScheduled(_)).WillByDefault(Return(false));
	ON_CALL(*program1, getCurrentScheduler()).WillByDefault(ReturnPointee(scheduler1));
	ON_CALL(*program2, isScheduled(_)).WillByDefault(Return(false));
	ON_CALL(*program2, getCurrentScheduler()).WillByDefault(ReturnPointee(scheduler2));

	program1->setAdjustment(adjustment1);
	program2->setAdjustment(adjustment2);

	irrigationDocument = IrrigationDocument::Builder().
		setWateringController(wateringController).
		setProgramContainer(shared_ptr<ProgramContainer>(new ProgramContainer({{ IdType(), program1 }, { IdType(), program2 }}))).
		build();

	view.reset(new TimerView(*irrigationDocument));
}

void TimerViewTest::TearDown() {

}

bool operator== (const tm& t1, const tm& t2) {
	return (t1.tm_year == t2.tm_year &&
			t1.tm_mon == t2.tm_mon &&
			t1.tm_mday == t2.tm_mday &&
			t1.tm_hour == t2.tm_hour &&
			t1.tm_min == t2.tm_min &&
			t1.tm_sec == t2.tm_sec);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerViewTest, notActiveNotScheduled) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawtime)).Times(1);
	EXPECT_CALL(*program1, isScheduled(timeinfo)).Times(1);
	EXPECT_CALL(*program2, isScheduled(timeinfo)).Times(1);

	view->onTimer(rawtime);
}

TEST_F(TimerViewTest, notActiveScheduledFirst) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*wateringController, start(rawtime, runTimes1, adjustment1)).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawtime)).Times(1);
	EXPECT_CALL(*program1, isScheduled(timeinfo)).Times(1).WillOnce(Return(true));

	view->onTimer(rawtime);
}

TEST_F(TimerViewTest, notActiveScheduledSecond) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1);
	EXPECT_CALL(*wateringController, start(rawtime, runTimes2, adjustment2)).Times(1);
	EXPECT_CALL(*wateringController, on1SecTimer(rawtime)).Times(1);
	EXPECT_CALL(*program1, isScheduled(timeinfo)).Times(1);
	EXPECT_CALL(*program2, isScheduled(timeinfo)).Times(1).WillOnce(Return(true));

	view->onTimer(rawtime);
}

TEST_F(TimerViewTest, active) {
	EXPECT_CALL(*wateringController, isWateringActive()).Times(1).WillOnce(Return(true));
	EXPECT_CALL(*wateringController, on1SecTimer(rawtime)).Times(1);

	view->onTimer(rawtime);
}
