#include <gmock/gmock.h>
#include "Views/RestView/XmlTemperatureWriter.h"

using namespace std;
using namespace testing;


TEST(XmlTemperatureWriterTest, temperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<current-temperature>
	<value>28.0</value>
	<unit>celsius</unit>
</current-temperature>
)";

	EXPECT_THAT(XmlTemperatureWriter().currentToString(28), Eq(expected));
}

TEST(XmlTemperatureWriterTest, temperatureForecast) {
	const std::string expected =
R"(<?xml version="1.0"?>
<temperature-forecast>
	<min>10.0</min>
	<max>20.0</max>
	<unit>celsius</unit>
	<from>Sun, 30 May 2021 00:00:00 +0200</from>
	<to>Sun, 30 May 2021 23:59:59 +0200</to>
</temperature-forecast>
)";

	const TemperatureForecast::Values value(10, 20);
	const LocalDateTime from = LocalDateTime::create(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to = LocalDateTime::create(2021, 5, 30, 23, 59, 59);

	EXPECT_THAT(XmlTemperatureWriter().forecastToString(value, from, to), Eq(expected));
}

TEST(XmlTemperatureWriterTest, temperatureHistory) {
	const std::string expected =
R"(<?xml version="1.0"?>
<temperature-history>
	<min>10.0</min>
	<max>20.0</max>
	<unit>celsius</unit>
	<from>Sun, 30 May 2021 00:00:00 +0200</from>
	<to>Sun, 30 May 2021 23:59:59 +0200</to>
</temperature-history>
)";

	const TemperatureHistory::Values value(10, 20, 18);
	const LocalDateTime from = LocalDateTime::create(2021, 5, 30, 0, 0, 0);
	const LocalDateTime to = LocalDateTime::create(2021, 5, 30, 23, 59, 59);

	EXPECT_THAT(XmlTemperatureWriter().historyToString(value, from, to), Eq(expected));
}
