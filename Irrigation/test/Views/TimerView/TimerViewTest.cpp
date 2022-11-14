#include "TimerViewTest.h"
#include "TestCommon/cout.h"

using namespace testing;


void TimerViewTest::SetUp() {
	mockTimefunc = std::make_shared<StrictMock<MockTimefunc>>();
	mockIrrigationDocument = std::make_shared<StrictMock<MockIrrigationDocument>>();
	view = std::make_shared<TimerView>(*mockIrrigationDocument);

	DateTime::setTimefunc(mockTimefunc);

	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(AnyNumber());
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(AnyNumber());
}

void TimerViewTest::TearDown() {
	DateTime::resetTimefunc();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerViewTest, callStartIfScheduled) {
	const LocalDateTime now(2021, 6, 22, 23, 36, 57);

	EXPECT_CALL(*mockTimefunc, getTime()).WillRepeatedly(Return(now.toRawTime()));
	EXPECT_CALL(*mockIrrigationDocument, startIfScheduled(now)).Times(1);

	view->run();
}

TEST_F(TimerViewTest, callStartIfScheduled_onceInAMinute) {
	const LocalDateTime now(2021, 6, 22, 23, 36, 0);

	EXPECT_CALL(*mockTimefunc, getTime()).
			Times(6).
			WillOnce(Return(now.addSeconds(0).toRawTime())). 	// run()
			WillOnce(Return(now.addSeconds(1).toRawTime())).	// run()
			WillOnce(Return(now.addSeconds(2).toRawTime())).	// run()
			WillOnce(Return(now.addSeconds(3).toRawTime())).	// run()
			WillOnce(Return(now.addSeconds(10).toRawTime())).	// run()
			WillOnce(Return(now.addSeconds(59).toRawTime()));	// run()



	EXPECT_CALL(*mockIrrigationDocument, startIfScheduled(now)).Times(1);

	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
}

TEST_F(TimerViewTest, callStartIfScheduled_onceInEveryMinute) {
	const LocalDateTime now1(2021, 6, 22, 23, 35, 59);
	const LocalDateTime now2(2021, 6, 22, 23, 36, 0);
	const LocalDateTime now3(2021, 6, 22, 23, 37, 0);

	EXPECT_CALL(*mockTimefunc, getTime()).
			Times(8).
			WillOnce(Return(now1.toRawTime())). 				// run()
			WillOnce(Return(now2.addSeconds(0).toRawTime())). 	// run()
			WillOnce(Return(now2.addSeconds(1).toRawTime())).	// run()
			WillOnce(Return(now2.addSeconds(2).toRawTime())).	// run()
			WillOnce(Return(now2.addSeconds(3).toRawTime())).	// run()
			WillOnce(Return(now2.addSeconds(10).toRawTime())).	// run()
			WillOnce(Return(now2.addSeconds(59).toRawTime())).	// run()
			WillOnce(Return(now3.toRawTime()));					// run()


	EXPECT_CALL(*mockIrrigationDocument, startIfScheduled(now1)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, startIfScheduled(now2)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, startIfScheduled(now3)).Times(1);

	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
	view->run();
}

