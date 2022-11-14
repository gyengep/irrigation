#include "IrrigationDocumentImpl.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Logic/ProgramImplBuilder.h"
#include "Logic/ProgramContainerImpl.h"
#include "Logic/WateringControllerImpl.h"


IrrigationDocumentImpl::Builder& IrrigationDocumentImpl::Builder::setProgramContainer(const std::shared_ptr<ProgramContainer>& programContainer) {
	this->programContainer = programContainer;
	return *this;
}

IrrigationDocumentImpl::Builder& IrrigationDocumentImpl::Builder::setProgramFactory(const std::shared_ptr<ProgramFactory>& programFactory) {
	this->programFactory = programFactory;
	return *this;
}

IrrigationDocumentImpl::Builder& IrrigationDocumentImpl::Builder::setWateringController(const std::shared_ptr<WateringController>& wateringController) {
	this->wateringController = wateringController;
	return *this;
}

IrrigationDocumentImpl::Builder& IrrigationDocumentImpl::Builder::setEmailHandler(const std::shared_ptr<EmailHandler>& emailHandler) {
	this->emailHandler = emailHandler;
	return *this;
}

std::shared_ptr<IrrigationDocumentImpl> IrrigationDocumentImpl::Builder::build() {

	if (nullptr == programContainer) {
		programContainer = std::make_shared<ProgramContainerImpl>();
	}

	if (nullptr == programFactory) {
		programFactory = ProgramImplFactory::Builder().build();
	}

	if (nullptr == wateringController) {
		wateringController = std::make_shared<WateringControllerImpl>(
				std::make_shared<ZoneHandlerImpl>(GpioValve::getValves())
			);
	}

	if (nullptr == emailHandler) {
		;
	}

	return std::make_shared<IrrigationDocumentImpl>(
		programContainer,
		programFactory,
		wateringController,
		emailHandler
	);
}
