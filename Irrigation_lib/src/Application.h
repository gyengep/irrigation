/*
 * Application.h
 *
 *  Created on: 2016. m�j. 15.
 *      Author: Rendszergazda
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


class Document;

class Application {
	Document* document;
	volatile bool isTerminated;

public:
	Application();
	~Application();

	void init();
	void run();
	void uninit();
	void terminate();
	
	time_t getTime() const;
};

Application* getApplication();

#endif /* APPLICATION_H_ */
