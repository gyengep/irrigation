#pragma once
#include "Samples/CurrentTemperatureSample.h"
#include "RestViewTest.h"


class RestViewCurrentTemperatureTest : public RestViewTest {
protected:

	static const RestViewTestSamples::CurrentTemperatureSample sample;
	static const std::string styleSheetFile;
    static std::string createCurrentTemperatureUrl(const std::string& requestParameters = "");
};
