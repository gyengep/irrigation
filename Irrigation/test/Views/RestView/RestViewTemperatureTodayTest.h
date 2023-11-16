#pragma once
#include "RestViewTemperatureTestBase.h"


class RestViewTemperatureTodayTest : public RestViewTemperatureTestBase {
protected:

    virtual void SetUp();
    virtual void TearDown();

	static const LocalDateTime now;
	static const LocalDateTime from;
	static const LocalDateTime to;

	static const std::string styleSheetFile;
};
