#include <gmock/gmock.h>
#include <stdexcept>
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Mocks/MockValve.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ZoneHandlerImplTest, getZoneCount) {
	EXPECT_EQ(6, ZoneHandler::getZoneCount());
}

TEST(ZoneHandlerImplTest, activate) {
	std::shared_ptr<MockValveFactory<NiceMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<NiceMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	for (size_t i = 0; i < zoneHandler->getZoneCount(); i++) {
		EXPECT_NO_THROW(zoneHandler->activate(i));
	}
}

TEST(ZoneHandlerImplTest, activateInvalid) {
	std::shared_ptr<MockValveFactory<NiceMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<NiceMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	EXPECT_THROW(zoneHandler->activate(zoneHandler->getZoneCount()), IndexOutOfBoundsException);
}

TEST(ZoneHandlerImplTest, getActiveId) {
	std::shared_ptr<MockValveFactory<NiceMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<NiceMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	EXPECT_EQ(ZoneHandler::invalidZoneId, zoneHandler->getActiveId());
	zoneHandler->activate(0);
	EXPECT_EQ(0, zoneHandler->getActiveId());
}

TEST(ZoneHandlerImplTest, activateValve) {
	std::shared_ptr<MockValveFactory<StrictMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<StrictMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	Sequence seq;

	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(0);
	zoneHandler->deactivate();
}

TEST(ZoneHandlerImplTest, deactivateInDestructor) {
	std::shared_ptr<MockValveFactory<StrictMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<StrictMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	Sequence seq;

	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[0], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(0);
}

TEST(ZoneHandlerImplTest, deactivateValve) {
	std::shared_ptr<MockValveFactory<StrictMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<StrictMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	Sequence seq;

	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(1);
	zoneHandler->deactivate();
	zoneHandler->activate(2);
	zoneHandler->deactivate();
}

TEST(ZoneHandlerImplTest, activateValveAgain) {
	std::shared_ptr<MockValveFactory<StrictMock<MockValve>>> mockValveFactory;
	std::shared_ptr<ZoneHandlerImpl> zoneHandler;

	mockValveFactory = make_shared<MockValveFactory<StrictMock<MockValve>>>();
	zoneHandler = ZoneHandlerImpl::Builder().setValveFactory(mockValveFactory).build();

	Sequence seq;

	EXPECT_CALL(*mockValveFactory->mockMasterValve, activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockZoneValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValveFactory->mockMasterValve, deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(1);
	zoneHandler->activate(2);
	zoneHandler->deactivate();
}
