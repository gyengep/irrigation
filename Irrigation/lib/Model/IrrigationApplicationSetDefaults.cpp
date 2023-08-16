#include "IrrigationApplication.h"
#include "Logic/ProgramImpl.h"
#include "Logic/RunTimeImpl.h"
#include "Logic/StartTimeImpl.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Logic/SchedulerContainerImpl.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Temperature/TemperatureHandler.h"


void IrrigationApplication::setMyDefaults() {
	irrigationDocument->getProgramContainer().insert(
		IdType(),
		std::make_shared<ProgramImpl>(true, "Fűlocsolás", 100, SchedulerType::TEMPERATURE_DEPENDENT,
			std::make_shared<SchedulerContainerImpl>(
				std::make_shared<EveryDaySchedulerImpl>(),
				std::make_shared<HotWeatherSchedulerImpl>(
					temperatureHandler->getTemperatureHistory()
				),
				std::make_shared<TemperatureDependentSchedulerImpl>(
					temperatureHandler->getTemperatureForecast(),
					temperatureHandler->getTemperatureHistory(),
					0.75f,
					50, 0
				),
				std::make_shared<WeeklySchedulerImpl>()
			),

			std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(26)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(38)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(32)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			}),
			std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(4, 0) }
			})
		)
	);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		std::make_shared<ProgramImpl>(true, "Buxus", 100, SchedulerType::TEMPERATURE_DEPENDENT,
			std::make_shared<SchedulerContainerImpl>(
				std::make_shared<EveryDaySchedulerImpl>(),
				std::make_shared<HotWeatherSchedulerImpl>(
					temperatureHandler->getTemperatureHistory()
				),
				std::make_shared<TemperatureDependentSchedulerImpl>(
					temperatureHandler->getTemperatureForecast(),
					temperatureHandler->getTemperatureHistory(),
					1.0f,
					100, 0
				),
				std::make_shared<WeeklySchedulerImpl>()
			),

			std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(20)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			}),
			std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(6, 0) }
			})
		)
	);


	irrigationDocument->getProgramContainer().insert(
		IdType(),
		std::make_shared<ProgramImpl>(true, "Kánikula", 100, SchedulerType::HOT_WEATHER,
			std::make_shared<SchedulerContainerImpl>(
				std::make_shared<EveryDaySchedulerImpl>(),
				std::make_shared<HotWeatherSchedulerImpl>(
					temperatureHandler->getTemperatureHistory(),
					std::chrono::hours(2), 33
				),
				std::make_shared<TemperatureDependentSchedulerImpl>(
					temperatureHandler->getTemperatureForecast(),
					temperatureHandler->getTemperatureHistory()
				),
				std::make_shared<WeeklySchedulerImpl>()
			),

			std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(2) + std::chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3) + std::chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			}),
			std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(12, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(13, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(14, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(15, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(16, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(17, 0) }
			})
		)
	);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		std::make_shared<ProgramImpl>(true, "Paradicsom", 100, SchedulerType::TEMPERATURE_DEPENDENT,
			std::make_shared<SchedulerContainerImpl>(
				std::make_shared<EveryDaySchedulerImpl>(),
				std::make_shared<HotWeatherSchedulerImpl>(
					temperatureHandler->getTemperatureHistory()
				),
				std::make_shared<TemperatureDependentSchedulerImpl>(
					temperatureHandler->getTemperatureForecast(),
					temperatureHandler->getTemperatureHistory(),
					1.0f,
					75, 75
				),
				std::make_shared<WeeklySchedulerImpl>()
			),

			std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(1)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			}),
			std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(7, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(20, 0) }
			})
		)
	);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		std::make_shared<ProgramImpl>(true, "Virág", 100, SchedulerType::TEMPERATURE_DEPENDENT,
			std::make_shared<SchedulerContainerImpl>(
				std::make_shared<EveryDaySchedulerImpl>(),
				std::make_shared<HotWeatherSchedulerImpl>(
					temperatureHandler->getTemperatureHistory()
				),
				std::make_shared<TemperatureDependentSchedulerImpl>(
					temperatureHandler->getTemperatureForecast(),
					temperatureHandler->getTemperatureHistory(),
					1.0f,
					75, 75
				),
				std::make_shared<WeeklySchedulerImpl>()
			),

			std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(16))
			}),
			std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(7, 10) },
				{ IdType(), std::make_shared<StartTimeImpl>(20, 10) }
			})
		)
	);

	irrigationDocument->setModified();
}
