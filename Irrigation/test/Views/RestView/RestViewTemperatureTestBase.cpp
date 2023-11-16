#include "RestViewTemperatureTestBase.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureTestBase::SetUp() {
	RestViewTest::SetUp();

	mockTimefunc = std::make_shared<StrictMock<MockTimefunc>>();
	DateTime::setTimefunc(mockTimefunc);
}

void RestViewTemperatureTestBase::TearDown() {
	DateTime::resetTimefunc();
}

