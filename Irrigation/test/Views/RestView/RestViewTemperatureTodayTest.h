#pragma once
#include "Samples/TemperatureTodaySample.h"
#include "RestViewTemperatureTestBase.h"


class RestViewTemperatureTodayTest : public RestViewTemperatureTestBase {
protected:

	static const Dto2XmlTest::TemperatureTodaySample sample;
	static const std::string styleSheetFile;
	static std::string createTemperatureTodayUrl(const std::string& requestParameters = "");
};
