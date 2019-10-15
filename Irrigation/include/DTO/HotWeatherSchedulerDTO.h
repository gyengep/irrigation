#pragma once
#include <memory>
#include <ostream>
#include "DTO.h"


class HotWeatherSchedulerDTO {
public:
	HotWeatherSchedulerDTO() = default;
	HotWeatherSchedulerDTO(HotWeatherSchedulerDTO&& other) = default;
	HotWeatherSchedulerDTO(const HotWeatherSchedulerDTO& other);
	HotWeatherSchedulerDTO(unsigned periodInSeconds, float minTemperature);

	HotWeatherSchedulerDTO& operator= (HotWeatherSchedulerDTO&&) = default;
	HotWeatherSchedulerDTO& operator= (const HotWeatherSchedulerDTO&) = delete;
	bool operator== (const HotWeatherSchedulerDTO& other) const;

	DECLARE_DTO_VALUE_COPY(HotWeatherSchedulerDTO, float, MinTemperature);
	DECLARE_DTO_VALUE_COPY(HotWeatherSchedulerDTO, unsigned, PeriodInSeconds);

	friend std::ostream& operator<<(std::ostream& os, const HotWeatherSchedulerDTO& scheduler);
};
