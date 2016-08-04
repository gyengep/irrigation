

#include "common.h"
#include "Application.h"

#include "CommandLineView.h"
#include "Document.h"
#include "Valve.h"


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
	puts("Application::Init");
	Valve::init();
	
	document = new Document();
	new CommandLineView(document);
}

void Application::run() {
	puts("Application::run __BEGIN__");
	
	while (!isTerminated) {
		document->doTask();
/*
		for(int i = 0; i < 6; i++) {
			document->openValve(i, true);
			usleep(50000);
			document->openValve(i, false);
		}
		for(int i = 6; i > 0; i--) {
			document->openValve(i, true);
			usleep(50000);
			document->openValve(i, false);
		}
*/
		usleep(50000);
	}
	
	puts("Application::run __END__");
}

void Application::uninit() {
	puts("Application::Uninit");
	delete document;
}

void Application::terminate() { 
	puts("Application::Terminated");
	isTerminated = true; 
}

std::time_t Application::getTime() const {
	return std::time(NULL);
}
