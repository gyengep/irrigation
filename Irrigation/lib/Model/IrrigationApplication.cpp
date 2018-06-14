#include "IrrigationApplication.h"
#include <chrono>
#include <mutex>
#include <sys/unistd.h>
#include "Hardware/Valves.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlWriter.h"


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
}

void Application::run() {

	init();
	LOGGER.info("Application started");

	auto updateTimePoint = chrono::steady_clock::now();
	time_t lastTime = 0;

	while (!isTerminated) {
		time_t currentTime = getTime();

		if ((currentTime != (lastTime + 1)) && (lastTime != 0)) {
			LOGGER.warning("Update period failure! different is: %d", (currentTime - lastTime));
		}

		lastTime = currentTime;
		document->on1SecTimer(currentTime);

		updateTimePoint += chrono::seconds(1);
		this_thread::sleep_until(updateTimePoint);
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
