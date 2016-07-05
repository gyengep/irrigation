/*
 * exception.h
 *
 *  Created on: 2016. júl. 1.
 *      Author: Rendszergazda
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>


class not_found_exception : public std::runtime_error {
public:
  explicit not_found_exception() : std::runtime_error("") {}
  explicit not_found_exception(const char* what_arg) : std::runtime_error(what_arg) {}
};

#endif /* EXCEPTION_H_ */
