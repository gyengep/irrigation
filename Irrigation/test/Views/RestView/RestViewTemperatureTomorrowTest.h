#pragma once
#include "Samples/TemperatureTomorrowSample.h"
#include "RestViewTemperatureTestBase.h"


class RestViewTemperatureTomorrowTest : public RestViewTemperatureTestBase {
protected:

	static const RestViewTestSamples::TemperatureTomorrowSample sample;
	static const std::string styleSheetFile;
	static std::string createTemperatureTomorrowUrl(const std::string& requestParameters = "");
};
