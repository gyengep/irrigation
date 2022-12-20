#pragma once
#include "Samples/TemperatureYesterdaySample.h"
#include "RestViewTemperatureTestBase.h"


class RestViewTemperatureYesterdayTest : public RestViewTemperatureTestBase {
protected:

	static const Dto2XmlTest::TemperatureYesterdaySample sample;
	static const std::string styleSheetFile;
	static std::string createTemperatureYesterdayUrl(const std::string& requestParameters = "");
};
