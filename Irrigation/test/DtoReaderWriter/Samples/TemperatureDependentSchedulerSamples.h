#pragma once
#include <string>
#include <vector>
#include "DTO/TemperatureDependentSchedulerDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<TemperatureDependentSchedulerDto> TemperatureDependentSchedulerSample;

	class TemperatureDependentSchedulerSampleList : public std::vector<TemperatureDependentSchedulerSample> {
	public:
		TemperatureDependentSchedulerSampleList() {

			emplace_back(
				TemperatureDependentSchedulerDto(),
				"<scheduler type=\"temperature-dependent\"/>"
			);

			emplace_back(
				TemperatureDependentSchedulerDto().setRemainingCorrection(12.5f),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>12.50</remaining-correction>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDto().setMinAdjustment(95),
				"<scheduler type=\"temperature-dependent\">"
					"<min-adjustment>95</min-adjustment>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDto().setMaxAdjustment(96),
				"<scheduler type=\"temperature-dependent\">"
					"<max-adjustment>96</max-adjustment>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDto(2.5f, 85, 86),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>2.50</remaining-correction>"
					"<min-adjustment>85</min-adjustment>"
					"<max-adjustment>86</max-adjustment>"
				"</scheduler>"
			);
		}
	};
};
