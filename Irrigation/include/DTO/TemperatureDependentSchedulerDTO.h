#pragma once
#include <memory>
#include <ostream>
#include "DTO.h"


class TemperatureDependentSchedulerDTO {
public:
	TemperatureDependentSchedulerDTO() = default;
	TemperatureDependentSchedulerDTO(TemperatureDependentSchedulerDTO&& other) = default;
	TemperatureDependentSchedulerDTO(const TemperatureDependentSchedulerDTO& other);
	TemperatureDependentSchedulerDTO(float remainingA, float forecastA, float forecastB, float historyA, float historyB,
		int minAdjustment, int maxAdjustment, int trim);

	TemperatureDependentSchedulerDTO& operator= (TemperatureDependentSchedulerDTO&&) = default;
	TemperatureDependentSchedulerDTO& operator= (const TemperatureDependentSchedulerDTO&) = delete;
	bool operator== (const TemperatureDependentSchedulerDTO& other) const;

	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, RemainingA);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, ForecastA);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, ForecastB);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, HistoryA);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, HistoryB);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MinAdjustment);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MaxAdjustment);
	DECLARE_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, Trim);

	friend std::ostream& operator<<(std::ostream& os, const TemperatureDependentSchedulerDTO& scheduler);
};
