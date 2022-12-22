#pragma once
#include "DTO/TemperatureHistoryDTO.h"
#include "Utils/DateTime.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	class TemperatureYesterdaySample  : public Sample<TemperatureHistoryDTO> {
		const LocalDateTime now;
		const LocalDateTime from;
		const LocalDateTime to;

	public:
		TemperatureYesterdaySample() : Sample<TemperatureHistoryDTO>(
				TemperatureHistoryDTO("Mon, 21 Jun 2021 00:00:00 +0200", "Mon, 21 Jun 2021 23:59:59 +0200", "celsius", 25, 30, 28),
				"<temperature>"
					"<unit>celsius</unit>"
					"<historical>"
						"<value-min>25</value-min>"
						"<value-max>30</value-max>"
						"<value-avg>28</value-avg>"
						"<datetime-from>Mon, 21 Jun 2021 00:00:00 +0200</datetime-from>"
						"<datetime-to>Mon, 21 Jun 2021 23:59:59 +0200</datetime-to>"
					"</historical>"
				"</temperature>"
			),
			now(2021, 6, 22, 12, 21, 22),
			from(2021, 6, 21, 0, 0, 0),
			to(2021, 6, 21, 23, 59, 59)
		{
		}

		const DateTime& getNow() const  { return now; }
		const DateTime& getFrom() const { return from; }
		const DateTime& getTo() const   { return to; }
	};
}

