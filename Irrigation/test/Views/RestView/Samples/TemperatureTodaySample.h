#pragma once
#include <memory>
#include <vector>
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"


namespace Dto2XmlTest {

	class TemperatureTodaySample {
		const LocalDateTime now;
		const LocalDateTime from;
		const LocalDateTime to;
		const TemperatureForecast::Values forecastValues;
		const TemperatureHistory::Values historyValues;
		const std::string xml;

		std::vector<std::pair<std::string, std::string>> dateTimeFormatAndXml;

	public:
		TemperatureTodaySample() :
			now(2021, 5, 29, 19, 33, 54),
			from(2021, 5, 29, 0, 0, 0),
			to(2021, 5, 29, 23, 59, 59),
			forecastValues(25, 30),
			historyValues(20, 28, 23),
			xml("<temperature>"
					"<unit>celsius</unit>"
					"<historical>"
						"<value-min>20</value-min>"
						"<value-max>28</value-max>"
						"<datetime-from>Sat, 29 May 2021 00:00:00 +0200</datetime-from>"
						"<datetime-to>Sat, 29 May 2021 19:33:54 +0200</datetime-to>"
					"</historical>"
					"<forecast>"
						"<value-min>25</value-min>"
						"<value-max>30</value-max>"
						"<datetime-from>Sat, 29 May 2021 19:33:54 +0200</datetime-from>"
						"<datetime-to>Sat, 29 May 2021 23:59:59 +0200</datetime-to>"
					"</forecast>"
				"</temperature>")
		{
			dateTimeFormatAndXml.push_back(
					std::make_pair(
							"", xml
						)
				);

			dateTimeFormatAndXml.push_back(
					std::make_pair(
							"datetime-format=%Y-%m-%d",
							"<temperature>"
								"<unit>celsius</unit>"
								"<historical>"
									"<value-min>20</value-min>"
									"<value-max>28</value-max>"
									"<datetime-from>2021-05-29</datetime-from>"
									"<datetime-to>2021-05-29</datetime-to>"
								"</historical>"
								"<forecast>"
									"<value-min>25</value-min>"
									"<value-max>30</value-max>"
									"<datetime-from>2021-05-29</datetime-from>"
									"<datetime-to>2021-05-29</datetime-to>"
								"</forecast>"
							"</temperature>"
						)
				);
}

		const LocalDateTime& getNow() const 				 { return now; }
		const LocalDateTime& getFrom() const 				 { return from; }
		const LocalDateTime& getTo() const  	        	 { return to; }
		const std::string&   getXml() const				 	 { return xml; }

		const TemperatureHistory::Values& getHistoryValues() const   { return historyValues; }
		const TemperatureForecast::Values& getForecastValues() const { return forecastValues; }

		const std::pair<std::string, std::string>& getDateTimeFormatAndXml(size_t idx) const	{ return dateTimeFormatAndXml[idx]; }
	};
}

