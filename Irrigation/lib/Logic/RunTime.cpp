#include "RunTime.h"

///////////////////////////////////////////////////////////////////////////////

bool RunTime::operator== (const RunTime& other) const {
	return (getMilliSeconds() == other.getMilliSeconds());
}
