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

TEST_F(ZoneHandlerTestWithMock, activateValves) {
	EXPECT_CALL(*mockValves, activate(vector<size_t> { 0, 6 }, true)).Times(1);

	zones.activate(0);
}

TEST_F(ZoneHandlerTestWithMock, deactivateSetPin) {
	Sequence seq;

	EXPECT_CALL(*mockValves, activate(vector<size_t> { 1, 6 }, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activate(vector<size_t> { 1, 6 }, false)).Times(1).InSequence(seq);

	zones.activate(1);
	zones.deactivate();
}

TEST_F(ZoneHandlerTestWithMock, activateAgainSetPin) {
	Sequence seq;

	EXPECT_CALL(*mockValves, activate(vector<size_t> { 2, 6 }, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activate(vector<size_t> { 2, 6 }, false)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activate(vector<size_t> { 3, 6 }, true)).Times(1).InSequence(seq);

	zones.activate(2);
	zones.activate(3);
}

TEST(ZonesTest, deactivateAtDelete) {
	MockValves* mockValves = new MockValves();
	Valves::setNewInstance(mockValves);

	Sequence seq;

	EXPECT_CALL(*mockValves, activate(vector<size_t> { 4, 6 }, true)).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves, activate(vector<size_t> { 4, 6 }, false)).Times(1).InSequence(seq);

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

TEST(ZonesTest, getActiveIdDefault) {
	ZoneHandler zones;
	EXPECT_EQ(ZoneHandler::invalidZoneId, zones.getActiveId());
}

TEST(ZonesTest, getActiveIdChanged) {
	ZoneHandler zones;
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
