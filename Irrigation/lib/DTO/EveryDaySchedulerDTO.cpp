#include "EveryDaySchedulerDTO.h"

using namespace std;


bool EveryDaySchedulerDTO::operator== (const EveryDaySchedulerDTO& other) const {
	return true;
}

ostream& operator<<(ostream& os, const EveryDaySchedulerDTO& scheduler) {
	os << "EveryDaySchedulerDTO{}";
	return os;
}
