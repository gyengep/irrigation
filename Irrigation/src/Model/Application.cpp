

#include "Commons/common.h"
#include "Model/Application.h"

#include "Hardware/Valve.h"
#include "Model/Document.h"
#include "Views/CommandLineView.h"
#include "Views/WebServerView.h"


Application* theApplication = NULL;

Application* getApplication() {
	return theApplication;
}

Application::Application() :
		document(NULL),
		isTerminated(false)	{
	theApplication = this;
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::init() {
	LOGGER.info("Application::Init()");
	Valve::init();
	
	document = new Document();
	new CommandLineView(document);
	new WebServerView(document);

	document->programs().getMutex().lock();

	{
		Program& program = document->programs().add();
		program.setName("fulocsolas");
		program.runTimes().set(0, 25);
		program.runTimes().set(1, 35);
		program.runTimes().set(2, 25);
		program.startTimes().add(240);
	}

	{
		Program& program = document->programs().add();
		program.setName("buxus");
		program.runTimes().set(3, 20);
		program.startTimes().add(360);
	}

	{
		Program& program = document->programs().add();
		program.setName("paradicsom");
		program.runTimes().set(4, 3);
		program.startTimes().add(480);
	}

	document->programs().getMutex().unlock();
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
