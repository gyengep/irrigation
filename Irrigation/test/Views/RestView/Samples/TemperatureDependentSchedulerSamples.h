#pragma once
#include <memory>
#include <vector>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;

	class TemperatureDependentSchedulerSampleList : public std::vector<TemperatureDependentSchedulerSample> {
	public:
		TemperatureDependentSchedulerSampleList() {

			emplace_back(
				TemperatureDependentSchedulerDTO(2.5f, 10, 20),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>2.50</remaining-correction>"
					"<min-adjustment>10</min-adjustment>"
					"<max-adjustment>20</max-adjustment>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDTO(2.3f, 15, 25),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>2.30</remaining-correction>"
					"<min-adjustment>15</min-adjustment>"
					"<max-adjustment>25</max-adjustment>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDTO(12.0f, 25, 26),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>12</remaining-correction>"
					"<min-adjustment>25</min-adjustment>"
					"<max-adjustment>26</max-adjustment>"
				"</scheduler>"
			);

			emplace_back(
				TemperatureDependentSchedulerDTO(13.0f, 27, 28),
				"<scheduler type=\"temperature-dependent\">"
					"<remaining-correction>13</remaining-correction>"
					"<min-adjustment>27</min-adjustment>"
					"<max-adjustment>28</max-adjustment>"
				"</scheduler>"
			);
		}
	};
};
