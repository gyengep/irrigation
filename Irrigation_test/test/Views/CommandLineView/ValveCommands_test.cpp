#include "common.h"
#include "Commands_test.h"

#include "Command/CommandExecutor.h"
#include "Logic/ContainerFactories.h"
#include "Hardware/Valves.h"

#define GET_VALVES_MOCK() dynamic_cast<ValvesMock&>(Valves::getInstance())


using namespace CommandLine;
using ::testing::_;


class ValvesMock : public Valves {
public:

	MOCK_METHOD2(activate, void(size_t valveID, bool active));
	MOCK_METHOD3(activate, void(size_t valveID1, size_t valveID2, bool active));
};

class ValvesMockFactory : public Factory<Valves> {
public:
	Valves* create() { return new ValvesMock(); }
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ResetValvesTest::SetUp() {
	Valves::setFactory(new ValvesMockFactory());
	CommandsTest<ResetValves>::SetUp();
}

void ResetValvesTest::TearDown() {
	Valves::setFactory(new ValvesFactory());
	CommandsTest<ResetValves>::TearDown();
}

TEST_F(ResetValvesTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooMuchArgumentsException);
}

TEST_F(ResetValvesTest, success) {
	ValvesMock& valvesMock = GET_VALVES_MOCK();

	for (size_t index = 0; index < VALVE_COUNT; index++) {
		EXPECT_CALL(valvesMock, activate(index, false)).Times(1);
	}

	EXPECT_NO_THROW(command->execute(Tokens()));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ValveTest::SetUp() {
	Valves::setFactory(new ValvesMockFactory());
	CommandsTest<Valve>::SetUp();
}

void ValveTest::TearDown() {
	Valves::setFactory(new ValvesFactory());
	CommandsTest<Valve>::TearDown();
}

TEST_F(ValveTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooFewArgumentsException);
}

TEST_F(ValveTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooMuchArgumentsException);
}

TEST_F(ValveTest, invalidValveID) {
	const std::string valveID("jhvh");
	const std::string onOff("on");
	EXPECT_THROW(command->execute(Tokens({valveID, onOff})), InvalidValveIdException);
}

TEST_F(ValveTest, invalidSwitch) {
	const std::string valveID("1");
	const std::string onOff("76");
	EXPECT_THROW(command->execute(Tokens({valveID, onOff})), InvalidParameterException);
}

TEST_F(ValveTest, success) {
	const IdType valveId = 1;
	const bool activate = true;

	ValvesMock& valvesMock = GET_VALVES_MOCK();

	EXPECT_CALL(valvesMock, activate(valveId, activate)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(valveId), activate ? "on" : "off"})));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ZoneTest::SetUp() {
	Valves::setFactory(new ValvesMockFactory());
	CommandsTest<Zone>::SetUp();
}

void ZoneTest::TearDown() {
	Valves::setFactory(new ValvesFactory());
	CommandsTest<Zone>::TearDown();
}

TEST_F(ZoneTest, fewArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1"})), TooFewArgumentsException);
}

TEST_F(ZoneTest, muchArgument) {
	EXPECT_THROW(command->execute(Tokens({"P1", "P2", "P3"})), TooMuchArgumentsException);
}

TEST_F(ZoneTest, invalidZoneID) {
	const std::string zoneID("jhvh");
	const std::string onOff("on");
	EXPECT_THROW(command->execute(Tokens({zoneID, onOff})), InvalidZoneIdException);
}

TEST_F(ZoneTest, invalidSwitch) {
	const std::string zoneID("1");
	const std::string onOff("76");
	EXPECT_THROW(command->execute(Tokens({zoneID, onOff})), InvalidParameterException);
}

TEST_F(ZoneTest, success) {
	const IdType valveId = ZONE3_VALVE_ID;
	const IdType masterValveId = MASTER_VALVE_ID;
	const bool activate = true;

	ValvesMock& valvesMock = GET_VALVES_MOCK();

	EXPECT_CALL(valvesMock, activate(valveId, masterValveId, activate)).Times(1);
	EXPECT_NO_THROW(command->execute(Tokens({std::to_string(valveId), activate ? "on" : "off"})));
}
