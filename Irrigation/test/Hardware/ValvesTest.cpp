#include "ValvesTest.h"
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves.h"


using namespace std;



TEST(ValvesTest, getCount) {
	EXPECT_EQ(7, Valves::getInstancePtr()->getCount());
}

TEST(ValvesTest, activate1Valve) {
	for (size_t i = 0; i < Valves::getInstancePtr()->getCount(); i++) {
		EXPECT_NO_THROW(Valves::getInstancePtr()->activate(i, false));
	}
}

TEST(ValvesTest, activate1ValveInvalid) {
	EXPECT_THROW(Valves::getInstancePtr()->activate(Valves::getInstancePtr()->getCount(), true), IndexOutOfBoundsException);
}

TEST(ValvesTest, activateMoreValves) {
	const size_t size = 4;
	size_t pins[size] { 5, 2, 3, 6 };
	EXPECT_NO_THROW(Valves::getInstancePtr()->activate(pins, size, true));
}

TEST(ValvesTest, activateMoreValvesInvalid) {
	const size_t size = 4;
	size_t pins[size] { 5, 2, 3, 7 };
	EXPECT_THROW(Valves::getInstancePtr()->activate(pins, size, true), IndexOutOfBoundsException);
}

///////////////////////////////////////////////////////////////////////////////

void ValvesTestWithMock::SetUp() {
	mockGpioHandler.reset(new MockGpioHandler());
	valves.reset(new Valves(mockGpioHandler));
}

void ValvesTestWithMock::TearDown() {
}


TEST_F(ValvesTestWithMock, set1Pin) {
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE0_PIN, 1)).Times(1);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE5_PIN, 0)).Times(1);

	valves->activate(0, true);
	valves->activate(5, false);
}

TEST_F(ValvesTestWithMock, setMorePin1) {
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE5_PIN, 1)).Times(1);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE2_PIN, 1)).Times(1);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE3_PIN, 1)).Times(1);

	const size_t size = 3;
	const size_t pins[size] { 5, 2, 3 };
	valves->activate(pins, size, true);
}

TEST_F(ValvesTestWithMock, setMorePin2) {
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE0_PIN, 0)).Times(1);
	EXPECT_CALL(*mockGpioHandler, setPin(VALVE4_PIN, 0)).Times(1);

	const size_t size = 2;
	const size_t pins[size] { 0, 4 };
	valves->activate(pins, size, false);
}
