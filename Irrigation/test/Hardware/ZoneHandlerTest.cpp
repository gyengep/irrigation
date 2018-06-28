#include "ZoneHandlerTest.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves.h"
#include <stdexcept>

using namespace std;
using ::testing::Sequence;
using ::testing::NiceMock;



void ZoneHandlerTest::SetUp() {
	mockGpioHandler.reset(new NiceMock<MockGpioHandler>());
	zoneHandler.reset(new ZoneHandler(shared_ptr<Valves>(new Valves(mockGpioHandler))));
}

void ZoneHandlerTest::TearDown() {
}


TEST_F(ZoneHandlerTest, initWithNull) {
	EXPECT_THROW(ZoneHandler(shared_ptr<Valves>()), invalid_argument);
}

TEST_F(ZoneHandlerTest, getCount) {
	EXPECT_EQ(6, zoneHandler->getCount());
}

TEST_F(ZoneHandlerTest, activate) {
	for (size_t i = 0; i < zoneHandler->getCount(); i++) {
		EXPECT_NO_THROW(zoneHandler->activate(i));
	}
}

TEST_F(ZoneHandlerTest, activateInvalid) {
	EXPECT_THROW(zoneHandler->activate(zoneHandler->getCount()), IndexOutOfBoundsException);
}

TEST_F(ZoneHandlerTest, getActiveId) {
	EXPECT_EQ(ZoneHandler::invalidZoneId, zoneHandler->getActiveId());

	zoneHandler->activate(0);
	EXPECT_EQ(0, zoneHandler->getActiveId());
}

TEST_F(ZoneHandlerTest, activateValve) {
	Sequence seq;

	EXPECT_CALL(*mockGpioHandler, setPin(VALVE0_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE0_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 0)).Times(1).InSequence(seq);

	zoneHandler->activate(0);
}

TEST_F(ZoneHandlerTest, deactivateValve) {
	Sequence seq;

	EXPECT_CALL(*mockGpioHandler, setPin(VALVE1_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE1_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE2_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE2_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 0)).Times(1).InSequence(seq);

	zoneHandler->activate(1);
	zoneHandler->deactivate();
	zoneHandler->activate(2);
}

TEST_F(ZoneHandlerTest, activateValveAgain) {
	Sequence seq;

	EXPECT_CALL(*mockGpioHandler, setPin(VALVE2_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE2_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE3_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 1)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE3_PIN, 0)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE6_PIN, 0)).Times(1).InSequence(seq);

	zoneHandler->activate(2);
	zoneHandler->activate(3);
}
