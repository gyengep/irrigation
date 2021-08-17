#include <gmock/gmock.h>
#include "Views/RestView/XmlTemperatureWriter.h"

using namespace std;
using namespace testing;


TEST(XmlTemperatureWriterTest, temperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/temperature-current.xsl"?>
<temperature>
	<value>28</value>
	<datetime>22:00</datetime>
	<unit>celsius</unit>
</temperature>
)";

	EXPECT_THAT(XmlTemperatureWriter().currentToString("22:00", 28), Eq(expected));
}

TEST(XmlTemperatureWriterTest, tomorrowTemperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/temperature-tomorrow.xsl"?>
<temperature>
	<unit>celsius</unit>
	<forecast>
		<value-min>10</value-min>
		<value-max>20</value-max>
		<datetime-from>2021-05-01 00:00:00</datetime-from>
		<datetime-to>2021-05-01 23:59:59</datetime-to>
	</forecast>
</temperature>
)";

	const TemperatureForecast::Values value(10, 20);

	EXPECT_THAT(XmlTemperatureWriter().tomorrowToString("2021-05-01 00:00:00", "2021-05-01 23:59:59", value), Eq(expected));
}

TEST(XmlTemperatureWriterTest, todayTemperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/temperature-today.xsl"?>
<temperature>
	<unit>celsius</unit>
	<historical>
		<value-min>15</value-min>
		<value-max>25</value-max>
		<datetime-from>2021-05-01 00:00:00</datetime-from>
		<datetime-to>2021-05-01 12:13:14</datetime-to>
	</historical>
	<forecast>
		<value-min>10</value-min>
		<value-max>20</value-max>
		<datetime-from>2021-05-01 12:13:14</datetime-from>
		<datetime-to>2021-05-01 23:59:59</datetime-to>
	</forecast>
</temperature>
)";

	const TemperatureForecast::Values forecastValue(10, 20);
	const TemperatureHistory::Values historyValue(15, 25, 22);

	EXPECT_THAT(XmlTemperatureWriter().todayToString("2021-05-01 00:00:00", "2021-05-01 12:13:14", "2021-05-01 23:59:59", historyValue, forecastValue), Eq(expected));
}

TEST(XmlTemperatureWriterTest, yesterdayTemperature) {
	const std::string expected =
R"(<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/temperature-yesterday.xsl"?>
<temperature>
	<unit>celsius</unit>
	<historical>
		<value-min>10</value-min>
		<value-max>20</value-max>
		<datetime-from>2021-02-03 00:00:00</datetime-from>
		<datetime-to>2021-02-03 23:59:59</datetime-to>
	</historical>
</temperature>
)";

	const TemperatureHistory::Values value(10, 20, 18);

	EXPECT_THAT(XmlTemperatureWriter().yesterdayToString("2021-02-03 00:00:00", "2021-02-03 23:59:59", value), Eq(expected));
}
