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



Application* theApplication = NULL;

Application* getApplication() {
	return theApplication;
}

Application::Application() :
	document(NULL),
	isTerminated(false)
{
	theApplication = this;
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::init() {
	LOGGER.info("Application::Init()");
	Valves::init();
	
	document = new IrrigationDocument();
	document->addView(new CommandLineView());
	document->addView(new WebServerView());

	document->load();
}

void Application::run() {
	LOGGER.info("Application::run __BEGIN__");
	
	while (!isTerminated) {
		document->doTask();
		usleep(50000);
	}
	
	LOGGER.info("Application::run __END__");
}

void Application::uninit() {
	LOGGER.info("Application::Uninit");
	delete document;
}

void Application::terminate() { 
	LOGGER.info("Application::Terminated");
	isTerminated = true; 
}

std::time_t Application::getTime() const {
	return std::time(NULL);
}
