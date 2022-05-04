#include <gmock/gmock.h>
#include <stdexcept>
#include "ZoneHandlerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void ZoneHandlerImplTest::SetUp() {
	mockValves = std::vector<std::shared_ptr<MockValve>>({
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>(),
		std::make_shared<StrictMock<MockValve>>()
	});

	zoneHandler = std::make_shared<ZoneHandlerImpl>(std::vector<std::shared_ptr<Valve>>(mockValves.begin(), mockValves.end()));
}

void ZoneHandlerImplTest::TearDown() {

}


TEST_F(ZoneHandlerImplTest, getZoneCount) {
	EXPECT_EQ(6, ZoneHandler::getZoneCount());
}

TEST_F(ZoneHandlerImplTest, activate) {

	for (size_t i = 0; i < mockValves.size(); i++) {
		EXPECT_CALL(*mockValves[i], activate()).Times(AnyNumber());
		EXPECT_CALL(*mockValves[i], deactivate()).Times(AnyNumber());
	}

	for (size_t i = 0; i < zoneHandler->getZoneCount(); i++) {
		EXPECT_NO_THROW(zoneHandler->activate(i));
	}
}

TEST_F(ZoneHandlerImplTest, activateInvalid) {
	EXPECT_THROW(zoneHandler->activate(zoneHandler->getZoneCount()), IndexOutOfBoundsException);
}

TEST_F(ZoneHandlerImplTest, getActiveId) {
	for (size_t i = 0; i < mockValves.size(); i++) {
		EXPECT_CALL(*mockValves[i], activate()).Times(AnyNumber());
		EXPECT_CALL(*mockValves[i], deactivate()).Times(AnyNumber());
	}

	EXPECT_EQ(ZoneHandler::invalidZoneId, zoneHandler->getActiveId());
	zoneHandler->activate(0);
	EXPECT_EQ(0, zoneHandler->getActiveId());
	zoneHandler->activate(4);
	EXPECT_EQ(4, zoneHandler->getActiveId());
}

TEST_F(ZoneHandlerImplTest, activateValve) {
	Sequence seq;

	EXPECT_CALL(*mockValves[6], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[0], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[0], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(0);
	zoneHandler->deactivate();
}

TEST_F(ZoneHandlerImplTest, deactivateInDestructor) {
	Sequence seq;

	EXPECT_CALL(*mockValves[6], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[0], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[0], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(0);
}

TEST_F(ZoneHandlerImplTest, deactivateValve) {
	Sequence seq;

	EXPECT_CALL(*mockValves[6], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(1);
	zoneHandler->deactivate();
	zoneHandler->activate(2);
	zoneHandler->deactivate();
}

TEST_F(ZoneHandlerImplTest, activateValveAgain) {
	Sequence seq;

	EXPECT_CALL(*mockValves[6], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[1], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[1], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[2], activate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[2], deactivate()).Times(1).InSequence(seq);
	EXPECT_CALL(*mockValves[6], deactivate()).Times(1).InSequence(seq);

	zoneHandler->activate(1);
	zoneHandler->activate(2);
	zoneHandler->deactivate();
}
