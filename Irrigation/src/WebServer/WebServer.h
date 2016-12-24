/*
 * WebServerView.h
 *
 *  Created on: 2016.11.10.
 *      Author: 502664609
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <microhttpd.h>
#include <set>


class Answer;
class Request;


class WebServer {
	static int answer_to_connection(void *cls, struct MHD_Connection *connection,
		const char *url, const char *method, const char *version,
		const char *upload_data, size_t *upload_data_size, void **con_cls);

	static std::set<WebServer*> instances;

	struct MHD_Daemon *daemon;

	virtual void onRequest(const Request& request) = 0;

public:
	WebServer(uint16_t port);
	virtual ~WebServer();

	void send(const Answer& answer);
};

#endif /* WEBSERVER_H_ */
