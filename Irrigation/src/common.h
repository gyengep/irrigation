
#ifndef COMMON_H_
#define COMMON_H_

#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

const unsigned ZONE_COUNT = 6;
const unsigned VALVE_COUNT = 7;

#define INVALID_PROGRAMID 	"Invalid programID"
#define INVALID_DAYID 		"Invalid dayID"
#define INVALID_RUNTIMEID 	"Invalid runTimeID"
#define INVALID_STARTTIMEID "Invalid startTimeID"
#define INVALID_ZONEID 		"Invalid zoneID"
#define INVALID_VALVEID 	"Invalid valveID"

typedef unsigned long IdType;

#endif /* COMMON_H_ */
