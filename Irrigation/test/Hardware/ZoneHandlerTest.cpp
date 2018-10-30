#include "ZoneHandlerTest.h"
#include <stdexcept>
#include "Exceptions/Exceptions.h"
#include "Hardware/ValveConfig.h"

using namespace std;
using ::testing::Sequence;


MockValveFactory::MockValveFactory() {
	mockZoneValves.resize(ZoneHandler::getZoneCount());

	for (size_t i = 0; i < mockZoneValves.size(); i++) {
		mockZoneValves[i] = new MockValve();
	}

	mockMasterValve = new MockValve();
}

unique_ptr<Valve> MockValveFactory::createValve(size_t id) {
	if (id < mockZoneValves.size()) {
		return unique_ptr<MockValve>(mockZoneValves[id]);
	} else if (id == mockZoneValves.size()) {
		return unique_ptr<MockValve>(mockMasterValve);
	} else {
		throw logic_error("MockValveFactory::createValve() id > mockZoneValves.size()");
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ZoneHandlerTest, getZoneCount) {
	EXPECT_EQ(6, ZoneHandler::getZoneCount());
}

TEST(ZoneHandlerTest, activate) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setFakeValveFactory()
		.build();

	for (size_t i = 0; i < zoneHandler->getZoneCount(); i++) {
		EXPECT_NO_THROW(zoneHandler->activate(i));
	}
}

TEST(ZoneHandlerTest, activateInvalid) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setFakeValveFactory()
		.build();

	EXPECT_THROW(zoneHandler->activate(zoneHandler->getZoneCount()), IndexOutOfBoundsException);
}

TEST(ZoneHandlerTest, getActiveId) {
	shared_ptr<ZoneHandler> zoneHandler = ZoneHandler::Builder()
		.setFakeValveFactory()
		.build();

	EXPECT_EQ(ZoneHandler::invalidZoneId, zoneHandler->getActiveId());

	zoneHandler->activate(0);
	EXPECT_EQ(0, zoneHandler->getActiveId());
}

TEST(ZoneHandlerTest, activateValve) {
	Sequence seq;
	unique_ptr<MockValveFactory> mockValveFactory(new MockValveFactory());

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
	unique_ptr<MockValveFactory> mockValveFactory(new MockValveFactory());

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
	unique_ptr<MockValveFactory> mockValveFactory(new MockValveFactory());

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
