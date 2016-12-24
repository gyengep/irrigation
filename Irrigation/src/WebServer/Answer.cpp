/*
 * Answer.cpp
 *
 *  Created on: 2016.12.16.
 *      Author: 502664609
 */

#include "Common.h"
#include "Answer.h"
#include "Request.h"


Answer::Answer(const Request& request, unsigned statusCode, const void* data, size_t dataLength) :
	request(request),
	statusCode(statusCode),
	data(data),
	dataLength(dataLength)
{
}

Answer::~Answer() {
}

size_t Answer::getDataLength() const {
	return dataLength;
}

const void* Answer::getData() const {
	return data;
}

void* Answer::getConnection() const {
	return request.getConnection();
}

unsigned Answer::getStatusCode() const {
	return statusCode;
}
