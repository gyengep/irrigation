#pragma once


struct TemperatureValues {
	float min, max, avg;

	TemperatureValues(float min, float max, float avg) :
		min(min),
		max(max),
		avg(avg)
	{
	}
};
