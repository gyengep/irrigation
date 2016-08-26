/*
 * exception.h
 *
 *  Created on: 2016. júl. 1.
 *      Author: Rendszergazda
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>


class invalid_id : public std::runtime_error {
public:
  explicit invalid_id() : std::runtime_error("") {}
  explicit invalid_id(const char* what_arg) : std::runtime_error(what_arg) {}
};

class not_locked : std::runtime_error {
public:
  explicit not_locked() : std::runtime_error("") {}
  explicit not_locked(const char* what_arg) : std::runtime_error(what_arg) {}
};

#endif /* EXCEPTION_H_ */
