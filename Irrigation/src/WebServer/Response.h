/*
 * Answer.h
 *
 *  Created on: 2016.12.16.
 *      Author: 502664609
 */

#ifndef ANSWER_H_
#define ANSWER_H_

class Request;


class Answer {
	const Request& request;
	unsigned statusCode;
	const void* data;
	size_t dataLength;

public:
	Answer(const Request& request, unsigned statusCode, const void* data, size_t dataLength);
	virtual ~Answer();

	size_t getDataLength() const;
	const void* getData() const;
	void* getConnection() const;
	unsigned getStatusCode() const;
};

#endif /* ANSWER_H_ */
