#pragma once
#include "Samples/TemperatureCurrentSample.h"
#include "RestViewTest.h"


class RestViewTemperatureCurrentTest : public RestViewTest {
protected:

	static const Dto2XmlTest::TemperatureCurrentSample sample;
	static const std::string styleSheetFile;
    static std::string createTemperatureCurrentUrl(const std::string& requestParameters = "");
};
