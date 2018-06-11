#include <IrrigationApplication.h>
#include <mutex>
#include <sys/unistd.h>
#include "Hardware/Valves.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlWriter.h"
#include "Views/CommandLineView/CommandLineView.h"
//#include "Views/CommandLineView/WebServerView.h"


using namespace std;


mutex Application::createMutex;
unique_ptr<Application> Application::instance;

const string Application::configFileName = "/tmp/irrigation.xml";


Application& Application::getInstance() {
	if (nullptr == instance) {
		lock_guard<mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Application());
		}
	}

	return *instance.get();
}

Application::Application() :
	isTerminated(false)
{
}

Application::~Application() {
}

void Application::init() {
	LOGGER.info("Application initializing");
	Valves::init();
	
	document.reset(new IrrigationDocument());
	//document->addView(new CommandLineView());
	//document->addView(new WebServerView());
}

void Application::run() {

	init();
	LOGGER.info("Application started");
	
	while (!isTerminated) {
		//document->on1SecTimer();
		usleep(50000);
	}
	
	cleanup();
}

void Application::cleanup() {
	document.reset();
	LOGGER.info("Application terminated");
}

void Application::terminate() { 
	LOGGER.debug("Received terminate");
	isTerminated = true;
}

time_t Application::getTime() const {
	return time(NULL);
}
