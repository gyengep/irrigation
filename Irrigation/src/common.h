
#ifndef COMMON_H_
#define COMMON_H_

#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#ifdef IRRIGATION_TEST
#include <gtest/gtest.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif // IRRIGATION_TEST

#include "Commons/exception.h"
#include "Commons/Logger.h"

const unsigned ZONE_COUNT = 6;
const unsigned VALVE_COUNT = 7;

#define INVALID_PROGRAMID 	"Invalid programID"
#define INVALID_DAYID 		"Invalid day"
#define INVALID_RUNTIMEID 	"Invalid runTimeID"
#define INVALID_STARTTIMEID "Invalid startTimeID"
#define INVALID_ZONEID 		"Invalid zoneID"
#define INVALID_VALVEID 	"Invalid valveID"
#define INVALID_SCHEDULER	"Invalid scheduler type"

typedef unsigned long IdType;

#endif /* COMMON_H_ */
