#include <gmock/gmock.h>
#include "Views/RestView/XmlTemperatureWriter.h"

using namespace std;
using namespace testing;


TEST(XmlTemperatureWriterTest, temperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/resources/temperature.xsl"?>
<temperature>
	<value>28</value>
	<datetime>22:00</datetime>
	<unit>celsius</unit>
</temperature>
)";

	EXPECT_THAT(XmlTemperatureWriter().currentToString("22:00", 28), Eq(expected));
}

TEST(XmlTemperatureWriterTest, temperatureForecast) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/resources/temperature-forecast.xsl"?>
<temperature-forecast>
	<value-min>10</value-min>
	<value-max>20</value-max>
	<unit>celsius</unit>
	<datetime-from>2021-05-01 00:00:00</datetime-from>
	<datetime-to>2021-05-01 23:59:59</datetime-to>
</temperature-forecast>
)";

	const TemperatureForecast::Values value(10, 20);

	EXPECT_THAT(XmlTemperatureWriter().forecastToString("2021-05-01 00:00:00", "2021-05-01 23:59:59", value), Eq(expected));
}

TEST(XmlTemperatureWriterTest, temperatureHistory) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/resources/temperature-history.xsl"?>
<temperature-history>
	<value-min>10</value-min>
	<value-max>20</value-max>
	<unit>celsius</unit>
	<datetime-from>2021-02-03 00:00:00</datetime-from>
	<datetime-to>2021-02-03 23:59:59</datetime-to>
</temperature-history>
)";

	const TemperatureHistory::Values value(10, 20, 18);

	EXPECT_THAT(XmlTemperatureWriter().historyToString("2021-02-03 00:00:00", "2021-02-03 23:59:59", value), Eq(expected));
}
