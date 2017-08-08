#include "common.h"
#include "Application.h"

#include <mutex>
#include <sys/unistd.h>

#include "Hardware/Valves.h"
#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"
#include "Views/CommandLineView/CommandLineView.h"
#include "Views/WebServerView/WebServerView.h"


std::mutex Application::createMutex;
std::unique_ptr<Factory<Application>> Application::factory(new ApplicationFactory());
std::unique_ptr<Application> Application::instance;


Application& Application::getInstance() {
	if (nullptr == instance) {
		std::lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(factory->create());
		}
	}

	return *instance;
}

void Application::setFactory(Factory<Application>* applicationFactory) {
	std::lock_guard<std::mutex> lock(createMutex);

	instance.reset(nullptr);
	factory.reset(applicationFactory);
}

Application::Application() :
	document(NULL),
	isTerminated(false)
{
}

Application::~Application() {
}

void Application::init() {
	LOGGER.debug("Application initializing");
	Valves::init();
	
	document = new IrrigationDocument();
	document->addView(new CommandLineView());
	document->addView(new WebServerView());

	document->load();
}

void Application::run() {
	LOGGER.info("Application started");
	LOGGER.debug("Application main loop start");
	
	while (!isTerminated) {
		//document->doTask();
		usleep(50000);
	}
	
	LOGGER.debug("Application main loop finish");
}

void Application::uninit() {
	delete document;
	LOGGER.info("Application terminated");
}

void Application::terminate() { 
	LOGGER.debug("Terminate");
	isTerminated = true; 
}

std::time_t Application::getTime() const {
	return std::time(NULL);
}
