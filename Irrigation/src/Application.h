/*
 * Application.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <atomic>
#include <ctime>

class Document;

class Application {
	Document* document;
	std::atomic_bool isTerminated;

public:
	Application();
	~Application();

	void init();
	void run();
	void uninit();
	void terminate();
	
	std::time_t getTime() const;
};

Application* getApplication();

#endif /* APPLICATION_H_ */
