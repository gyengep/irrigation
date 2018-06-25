#include "ZoneHandlerTest.h"
#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"

using namespace std;
using ::testing::Sequence;



void ZoneHandlerTestWithMock::SetUp() {
	mockValves = new MockValves();
	Valves::setNewInstance(mockValves);
}

void ZoneHandlerTestWithMock::TearDown() {
	Valves::setNewInstance(nullptr);
}

TEST_F(ZoneHandlerTestWithMock, activate) {
	Sequence seq;

	EXPECT_CALL(*mockValves, activatePin(0, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, true)).Times(1).InSequence(seq);

	zones.activate(0);
}

TEST_F(ZoneHandlerTestWithMock, deactivate) {
	Sequence seq;

	EXPECT_CALL(*mockValves, activatePin(1, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(1, false)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, false)).Times(1).InSequence(seq);

	zones.activate(1);
	zones.deactivate();
}

TEST_F(ZoneHandlerTestWithMock, activateAgain) {
	Sequence seq;

	EXPECT_CALL(*mockValves, activatePin(2, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(2, false)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, false)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(3, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, true)).Times(1).InSequence(seq);

	zones.activate(2);
	zones.activate(3);
}

TEST(ZonesTest, deactivateAtDelete) {
	MockValves* mockValves = new MockValves();
	Valves::setNewInstance(mockValves);

	Sequence seq;

	EXPECT_CALL(*mockValves, activatePin(4, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(4, false)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activatePin(6, false)).Times(1).InSequence(seq);

	{
		ZoneHandler zones;
		zones.activate(4);
	}

	Valves::setNewInstance(nullptr);
}

TEST(ZonesTest, getCount) {
	ZoneHandler zones;
	EXPECT_EQ(6, zones.getCount());
}

TEST(ZonesTest, getActiveId) {
	ZoneHandler zones;

	EXPECT_EQ(ZoneHandler::invalidZoneId, zones.getActiveId());

	zones.activate(0);
	EXPECT_EQ(0, zones.getActiveId());
}


TEST(ZonesTest, activate) {
	ZoneHandler zones;

	for (size_t i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(zones.activate(i));
	}
}

TEST(ZonesTest, activateInvalid) {
	ZoneHandler zones;
	EXPECT_THROW(zones.activate(ZONE_COUNT), IndexOutOfBoundsException);
}
