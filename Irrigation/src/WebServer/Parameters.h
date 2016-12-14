/*
 * Parameters.h
 *
 *  Created on: 2016.12.13.
 *      Author: 502664609
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_


#include <map>
#include <string>



class Parameters : public std::map<std::string, std::string> {

public:
	Parameters();
	virtual ~Parameters();

	void loadFromString(const std::string& text);
};

#endif /* PARAMETERS_H_ */
