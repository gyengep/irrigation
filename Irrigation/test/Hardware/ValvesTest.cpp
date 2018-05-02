#include <gmock/gmock.h>
#include <memory>
#include "Hardware/Valves.h"

using namespace std;



class MockValves : public Valves {
public:
	MOCK_METHOD2(setPin, void(int, int));
};


TEST(ValvesTest, activate) {
	for (size_t i = 0; i < Valves::getInstance().getCount(); i++) {
		EXPECT_NO_THROW(Valves::getInstance().activate(i, false));
	}
}

TEST(ValvesTest, invalid) {
	EXPECT_THROW(Valves::getInstance().activate(Valves::getInstance().getCount(), true), out_of_range);
}

TEST(ValvesTest, setPin_1pc1) {
	unique_ptr<MockValves> valves(new MockValves());
	EXPECT_CALL(*valves.get(), setPin(VALVE0_PIN, 1)).Times(1);
	valves->activate(0, true);
}

TEST(ValvesTest, setPin_1pc2) {
	unique_ptr<MockValves> valves(new MockValves());
	EXPECT_CALL(*valves.get(), setPin(VALVE5_PIN, 0)).Times(1);
	valves->activate(5, false);
}

TEST(ValvesTest, setPin_more_invalid) {
	unique_ptr<MockValves> valves(new MockValves());

	vector<size_t> pins {5, 2, 3, 7};
	EXPECT_THROW(valves->activate(pins.data(), pins.size(), true), out_of_range);
}

TEST(ValvesTest, setPin_more_1) {
	unique_ptr<MockValves> valves(new MockValves());

	EXPECT_CALL(*valves.get(), setPin(VALVE5_PIN, 1)).Times(1);
	EXPECT_CALL(*valves.get(), setPin(VALVE2_PIN, 1)).Times(1);
	EXPECT_CALL(*valves.get(), setPin(VALVE3_PIN, 1)).Times(1);

	vector<size_t> pins {5, 2, 3};
	valves->activate(pins.data(), pins.size(), true);
}

TEST(ValvesTest, setPin_more_2) {
	unique_ptr<MockValves> valves(new MockValves());

	EXPECT_CALL(*valves.get(), setPin(VALVE0_PIN, 0)).Times(1);
	EXPECT_CALL(*valves.get(), setPin(VALVE4_PIN, 0)).Times(1);

	vector<size_t> pins {0, 4};
	valves->activate(pins.data(), pins.size(), false);
}
