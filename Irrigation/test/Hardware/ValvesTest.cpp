#include "ValvesTest.h"
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves.h"


using namespace std;



void ValvesTestWithMock::SetUp() {
	mockValves = new MockValves();
	Valves::setNewInstance(mockValves);
}

void ValvesTestWithMock::TearDown() {
	Valves::setNewInstance(nullptr);
}



TEST(ValvesTest, getCount) {
	EXPECT_EQ(7, Valves::getInstance().getCount());
}

TEST(ValvesTest, activate) {
	for (size_t i = 0; i < Valves::getInstance().getCount(); i++) {
		EXPECT_NO_THROW(Valves::getInstance().activate(i, false));
	}
}

TEST(ValvesTest, activateInvalid) {
	EXPECT_THROW(Valves::getInstance().activate(VALVE_COUNT, true), IndexOutOfBoundsException);
}

TEST_F(ValvesTestWithMock, setPin_1pc1) {
	EXPECT_CALL(*mockValves, setPin(VALVE0_PIN, 1)).Times(1);
	mockValves->activate(0, true);
}

TEST_F(ValvesTestWithMock, setPin_1pc2) {
	EXPECT_CALL(*mockValves, setPin(VALVE5_PIN, 0)).Times(1);
	mockValves->activate(5, false);
}

TEST_F(ValvesTestWithMock, setPin_more_invalid) {
	const size_t size = 4;
	size_t pins[size] { 5, 2, 3, 7 };
	EXPECT_THROW(Valves::getInstance().activate(pins, size, true), IndexOutOfBoundsException);
}

TEST_F(ValvesTestWithMock, setPin_more_1) {
	EXPECT_CALL(*mockValves, setPin(VALVE5_PIN, 1)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE2_PIN, 1)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE3_PIN, 1)).Times(1);

	const size_t size = 3;
	size_t pins[size] { 5, 2, 3 };
	Valves::getInstance().activate(pins, size, true);
}

TEST_F(ValvesTestWithMock, setPin_more_2) {
	EXPECT_CALL(*mockValves, setPin(VALVE0_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE4_PIN, 0)).Times(1);

	const size_t size = 2;
	size_t pins[size] { 0, 4 };
	Valves::getInstance().activate(pins, size, false);
}


TEST_F(ValvesTestWithMock, resetAll) {
	EXPECT_CALL(*mockValves, setPin(VALVE0_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE1_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE2_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE3_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE4_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE5_PIN, 0)).Times(1);
	EXPECT_CALL(*mockValves, setPin(VALVE6_PIN, 0)).Times(1);

	Valves::getInstance().resetAll();
}
