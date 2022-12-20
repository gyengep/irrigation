#pragma once
#include <memory>
#include <vector>
#include "Utils/DateTime.h"


namespace Dto2XmlTest {

	class TemperatureCurrentSample {
		const LocalDateTime now;
		const float value;
		const std::string xml;

		std::vector<std::pair<std::string, std::string>> dateTimeFormatAndXml;

	public:
		TemperatureCurrentSample() :
			now(2021, 6, 22, 23, 36, 57),
			value(28.1),
			xml("<temperature><value>28</value><datetime>Tue, 22 Jun 2021 23:36:57 +0200</datetime><unit>celsius</unit></temperature>")
		{
			dateTimeFormatAndXml.push_back(
					std::make_pair(
							"", xml
						)
				);

			dateTimeFormatAndXml.push_back(
					std::make_pair(
							"datetime-format=%H:%M",
							"<temperature>"
								"<value>28</value>"
								"<datetime>23:36</datetime>"
								"<unit>celsius</unit>"
							"</temperature>"
						)
				);
}

		const LocalDateTime& getNow() const 		{ return now; }
		const float getValue() const        		{ return value; }
		const std::string& getXml() const			{ return xml; }

		const std::pair<std::string, std::string>& getDateTimeFormatAndXml(size_t idx) const	{ return dateTimeFormatAndXml[idx]; }
	};
}

