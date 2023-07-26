#include "IrrigationApplication.h"
#include "Logic/ProgramImplBuilder.h"
#include "Logic/RunTimeImpl.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Logic/StartTimeImpl.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Logic/SchedulerContainerImplBuilder.h"
#include "Logic/WateringController.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Temperature/TemperatureHandler.h"


void IrrigationApplication::setMyDefaults() {
	irrigationDocument->getProgramContainer().insert(
		IdType(),
		ProgramImpl::Builder().
			setName("Fűlocsolás").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setSchedulerContainer(SchedulerContainerImpl::Builder().
				setTemperatureDependentScheduler(std::make_shared<TemperatureDependentSchedulerImpl>(
					TemperatureHandler::getInstance().getTemperatureForecast(),
					TemperatureHandler::getInstance().getTemperatureHistory(),
					0.75f,
					50, 0
				)).
				build()
			).
			setRunTimeContainer(std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(26)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(38)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(32)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			})).
			setStartTimeContainer(std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(4, 0) }
			})).
			build()
		);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		ProgramImpl::Builder().
			setName("Buxus").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setSchedulerContainer(SchedulerContainerImpl::Builder().
				setTemperatureDependentScheduler(std::make_shared<TemperatureDependentSchedulerImpl>(
					TemperatureHandler::getInstance().getTemperatureForecast(),
					TemperatureHandler::getInstance().getTemperatureHistory(),
					1.0f,
					100, 0
				)).
				build()
			).
			setRunTimeContainer(std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(20)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			})).
			setStartTimeContainer(std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(6, 0) }
			})).
			build()
		);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		ProgramImpl::Builder().
			setName("Kánikula").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::HOT_WEATHER).
			setSchedulerContainer(SchedulerContainerImpl::Builder().
				setHotWeatherScheduler(std::make_shared<HotWeatherSchedulerImpl>(
					TemperatureHandler::getInstance().getTemperatureHistory(),
					std::chrono::hours(2), 33
				)).
				build()
			).
			setRunTimeContainer(std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(2) + std::chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3) + std::chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			})).
			setStartTimeContainer(std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(12, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(13, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(14, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(15, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(16, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(17, 0) }
			})).
			build()
		);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		ProgramImpl::Builder().
			setName("Paradicsom").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setSchedulerContainer(SchedulerContainerImpl::Builder().
				setTemperatureDependentScheduler(std::make_shared<TemperatureDependentSchedulerImpl>(
					TemperatureHandler::getInstance().getTemperatureForecast(),
					TemperatureHandler::getInstance().getTemperatureHistory(),
					1.0f,
					75, 75
				)).
				build()
			).
			setRunTimeContainer(std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(1)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0))
			})).
			setStartTimeContainer(std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(7, 0) },
				{ IdType(), std::make_shared<StartTimeImpl>(20, 0) }
			})).
			build()
		);

	irrigationDocument->getProgramContainer().insert(
		IdType(),
		ProgramImpl::Builder().
			setName("Virág").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setSchedulerContainer(SchedulerContainerImpl::Builder().
				setTemperatureDependentScheduler(std::make_shared<TemperatureDependentSchedulerImpl>(
					TemperatureHandler::getInstance().getTemperatureForecast(),
					TemperatureHandler::getInstance().getTemperatureHistory(),
					1.0f,
					75, 75
				)).
				build()
			).
			setRunTimeContainer(std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(std::chrono::minutes(16))
			})).
			setStartTimeContainer(std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				{ IdType(), std::make_shared<StartTimeImpl>(7, 10) },
				{ IdType(), std::make_shared<StartTimeImpl>(20, 10) }
			})).
			build()
		);

	irrigationDocument->setModified();
}
