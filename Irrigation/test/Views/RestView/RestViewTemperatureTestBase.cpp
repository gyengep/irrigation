#include "RestViewTemperatureTestBase.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureTestBase::SetUp() {
	RestViewTest::SetUp();

	mockTimefunc = std::make_shared<MockTimefunc>();
	DateTime::setTimefunc(mockTimefunc);
}

void RestViewTemperatureTestBase::TearDown() {
	DateTime::resetTimefunc();
}

