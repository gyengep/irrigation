#pragma once
#include "DTO/TemperatureForecastDTO.h"
#include "DTO/TemperatureHistoryDTO.h"
#include "Utils/DateTime.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class TemperatureTodaySample  : public Sample<std::pair<TemperatureHistoryDTO, TemperatureForecastDTO>> {
		const LocalDateTime now;
		const LocalDateTime from;
		const LocalDateTime to;

	public:
		TemperatureTodaySample() : Sample<std::pair<TemperatureHistoryDTO, TemperatureForecastDTO>>(
				{ TemperatureHistoryDTO("Sun, 30 May 2021 00:00:00 +0200", "Sun, 30 May 2021 12:21:22 +0200", "C", 20, 25, 23),
					TemperatureForecastDTO("Sun, 30 May 2021 12:21:22 +0200", "Sun, 30 May 2021 23:59:59 +0200", "C", 30, 35) },
				"<temperature>"
					"<unit>C</unit>"
					"<historical>"
						"<value-min>20</value-min>"
						"<value-max>25</value-max>"
						"<value-avg>23</value-avg>"
						"<datetime-from>Sun, 30 May 2021 00:00:00 +0200</datetime-from>"
						"<datetime-to>Sun, 30 May 2021 12:21:22 +0200</datetime-to>"
					"</historical>"
					"<forecast>"
						"<value-min>30</value-min>"
						"<value-max>35</value-max>"
						"<datetime-from>Sun, 30 May 2021 12:21:22 +0200</datetime-from>"
						"<datetime-to>Sun, 30 May 2021 23:59:59 +0200</datetime-to>"
					"</forecast>"
				"</temperature>"
			),
			now(2021, 5, 30, 12, 21, 22),
			from(2021, 5, 30, 0, 0, 0),
			to(2021, 5, 30, 23, 59, 59)
		{
		}

		const DateTime& getNow() const  { return now; }
		const DateTime& getFrom() const { return from; }
		const DateTime& getTo() const   { return to; }
	};
}

