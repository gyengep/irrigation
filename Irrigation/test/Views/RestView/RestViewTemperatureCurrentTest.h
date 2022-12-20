#pragma once
#include "Samples/TemperatureCurrentSample.h"
#include "RestViewTemperatureTestBase.h"


class RestViewTemperatureCurrentTest : public RestViewTemperatureTestBase {
protected:

	static const Dto2XmlTest::TemperatureCurrentSample sample;
	static const std::string styleSheetFile;
    static std::string createTemperatureCurrentUrl(const std::string& requestParameters = "");
};
