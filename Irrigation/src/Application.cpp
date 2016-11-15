

#include "common.h"
#include "Application.h"
#include "CommandLineView.h"
#include "Document.h"
#include "Valve.h"
#include "WebServerView.h"


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
	Program& p1 = document->programs().add();
	p1.setName("fu");
	p1.runTimes().set(0, 25);
	p1.runTimes().set(1, 35);
	p1.runTimes().set(2, 25);
	p1.startTimes().add(240);

	Program& p2 = document->programs().add();
	p2.setName("virag");
	p2.runTimes().set(3, 20);
	p2.startTimes().add(360);

	Program& p3 = document->programs().add();
	p3.setName("paradicsom");
	p3.runTimes().set(4, 3);
	p3.startTimes().add(420);
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
