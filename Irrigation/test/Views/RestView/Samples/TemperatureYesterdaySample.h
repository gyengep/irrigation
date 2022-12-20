#pragma once
#include <memory>
#include <vector>
#include "Temperature/TemperatureHistory.h"
#include "Utils/DateTime.h"


namespace Dto2XmlTest {

	class TemperatureYesterdaySample {
		const LocalDateTime now;
		const LocalDateTime from;
		const LocalDateTime to;
		const TemperatureHistory::Values historyValues;
		const std::string xml;

		std::vector<std::pair<std::string, std::string>> dateTimeFormatAndXml;

	public:
		TemperatureYesterdaySample() :
			now(2021, 6, 22, 23, 36, 57),
			from(2021, 6, 21, 0, 0, 0),
			to(2021, 6, 21, 23, 59, 59),
			historyValues(25, 30, 28),
			xml("<temperature>"
					"<unit>celsius</unit>"
					"<historical>"
						"<value-min>25</value-min>"
						"<value-max>30</value-max>"
						"<datetime-from>Mon, 21 Jun 2021 00:00:00 +0200</datetime-from>"
						"<datetime-to>Mon, 21 Jun 2021 23:59:59 +0200</datetime-to>"
					"</historical>"
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
									"<value-min>25</value-min>"
									"<value-max>30</value-max>"
									"<datetime-from>2021-06-21</datetime-from>"
									"<datetime-to>2021-06-21</datetime-to>"
								"</historical>"
							"</temperature>"
						)
				);
}

		const LocalDateTime& getNow() const 				{ return now; }
		const LocalDateTime& getFrom() const 				{ return from; }
		const LocalDateTime& getTo() const  	        	{ return to; }
		const std::string&  getXml() const					{ return xml; }

		const TemperatureHistory::Values& getHistoryValues() const { return historyValues; }

		const std::pair<std::string, std::string>& getDateTimeFormatAndXml(size_t idx) const	{ return dateTimeFormatAndXml[idx]; }
	};
}

