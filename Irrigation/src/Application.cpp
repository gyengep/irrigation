

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
	LOG_INFO("Application::Init()");
	Valve::init();
	
	document = new Document();
	new CommandLineView(document);
}

void Application::run() {
	LOG_INFO("Application::run __BEGIN__");
	
	while (!isTerminated) {
		document->doTask();
		usleep(50000);
	}
	
	LOG_INFO("Application::run __END__");
}

void Application::uninit() {
	LOG_INFO("Application::Uninit");
	delete document;
}

void Application::terminate() { 
	LOG_INFO("Application::Terminated");
	isTerminated = true; 
}

std::time_t Application::getTime() const {
	return std::time(NULL);
}
