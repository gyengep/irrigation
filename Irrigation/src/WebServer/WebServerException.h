/*
 * WebServerException.h
 *
 *  Created on: 2016.12.16.
 *      Author: 502664609
 */

#ifndef WEBSERVEREXCEPTION_H_
#define WEBSERVEREXCEPTION_H_

#include <stdexcept>


class WebServerException : public std::runtime_error {
public:
	WebServerException(const char* what_arg) : std::runtime_error(what_arg) {}
};

#endif /* WEBSERVEREXCEPTION_H_ */
