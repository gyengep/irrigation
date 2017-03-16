
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

#define VALVE0_PIN 4
#define VALVE1_PIN 5
#define VALVE2_PIN 6
#define VALVE3_PIN 26
#define VALVE4_PIN 27
#define VALVE5_PIN 28
#define VALVE6_PIN 29

#define ZONE0_VALVE_ID  0
#define ZONE1_VALVE_ID  1
#define ZONE2_VALVE_ID  2
#define ZONE3_VALVE_ID  3
#define ZONE4_VALVE_ID  4
#define ZONE5_VALVE_ID  5
#define MASTER_VALVE_ID 6

#define ZONE_VALVE_IDS { ZONE0_VALVE_ID, ZONE1_VALVE_ID, ZONE2_VALVE_ID, ZONE3_VALVE_ID, ZONE4_VALVE_ID, ZONE5_VALVE_ID}


#endif /* COMMON_H_ */
