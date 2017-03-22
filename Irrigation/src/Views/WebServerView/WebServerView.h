/*
 * WebServerView.h
 *
 *  Created on: 2016.12.20.
 *      Author: 502664609
 */

#ifndef WEBSERVERVIEW_H_
#define WEBSERVERVIEW_H_

#include "DocumentView/View.h"


class IrrigationWebServer;


class WebServerView: public View {
	IrrigationWebServer* webServer;

public:
	WebServerView();
	virtual ~WebServerView();
};

#endif /* WEBSERVERVIEW_H_ */
