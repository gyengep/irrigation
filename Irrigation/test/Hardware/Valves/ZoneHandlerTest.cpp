#include <stdexcept>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ValveConfig.h"
#include "MockValve.h"

using namespace std;
using ::testing::Sequence;
using ::testing::NiceMock;

///////////////////////////////////////////////////////////////////////////////

TEST(ZoneHandlerTest, getZoneCount) {
	EXPECT_EQ(6, ZoneHandler::getZoneCount());
}

TEST(ZoneHandlerTest, activate) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(unique_ptr<ValveFactory>(new MockValveFactory<NiceMock<MockValve>>()))
		.build();

	for (size_t i = 0; i < zoneHandler->getZoneCount(); i++) {
		EXPECT_NO_THROW(zoneHandler->activate(i));
	}
}

TEST(ZoneHandlerTest, activateInvalid) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(unique_ptr<ValveFactory>(new MockValveFactory<NiceMock<MockValve>>()))
		.build();

	EXPECT_THROW(zoneHandler->activate(zoneHandler->getZoneCount()), IndexOutOfBoundsException);
}

TEST(ZoneHandlerTest, getActiveId) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(unique_ptr<ValveFactory>(new MockValveFactory<NiceMock<MockValve>>()))
		.build();

	EXPECT_EQ(ZoneHandler::invalidZoneId, zoneHandler->getActiveId());

	zoneHandler->activate(0);
	EXPECT_EQ(0, zoneHandler->getActiveId());
}

TEST(ZoneHandlerTest, activateValve) {
	Sequence seq;
	unique_ptr<MockValveFactory<MockValve>> mockValveFactory(new MockValveFactory<MockValve>());

	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(move(mockValveFactory))
		.build();

	zoneHandler->activate(0);
}

TEST(ZoneHandlerTest, deactivateValve) {
	Sequence seq;
	unique_ptr<MockValveFactory<MockValve>> mockValveFactory(new MockValveFactory<MockValve>());

	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(move(mockValveFactory))
		.build();

	zoneHandler->activate(1);
	zoneHandler->deactivate();
	zoneHandler->activate(2);
}

TEST(ZoneHandlerTest, activateValveAgain) {
	Sequence seq;
	unique_ptr<MockValveFactory<MockValve>> mockValveFactory(new MockValveFactory<MockValve>());

	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setValveFactory(move(mockValveFactory))
		.build();

	zoneHandler->activate(1);
	zoneHandler->activate(2);
}
