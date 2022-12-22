#pragma once
#include "DTO/TemperatureForecastDTO.h"
#include "Utils/DateTime.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	class TemperatureTomorrowSample : public Sample<TemperatureForecastDTO> {
		const LocalDateTime now;
		const LocalDateTime from;
		const LocalDateTime to;

	public:
		TemperatureTomorrowSample() : Sample<TemperatureForecastDTO>(
				TemperatureForecastDTO("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 23:59:59 +0200", "C", 25, 30),
				"<temperature>"
					"<unit>C</unit>"
					"<forecast>"
						"<value-min>25</value-min>"
						"<value-max>30</value-max>"
						"<datetime-from>Sun, 30 May 2021 00:00:00 +0200</datetime-from>"
						"<datetime-to>Sun, 30 May 2021 23:59:59 +0200</datetime-to>"
					"</forecast>"
				"</temperature>"
			),
			now(2021, 5, 29, 12, 21, 22),
			from(2021, 5, 30, 0, 0, 0),
			to(2021, 5, 30, 23, 59, 59)
		{
		}

		const DateTime& getNow() const  { return now; }
		const DateTime& getFrom() const { return from; }
		const DateTime& getTo() const   { return to; }
	};
}

