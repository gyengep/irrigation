#include "IdType.h"
#include <algorithm>
#include <sstream>


using namespace std;


unsigned IdType::lastUsedId = 0;


IdType::IdType() :
	IdType(lastUsedId + 1)
{
}

IdType::IdType(unsigned id) :
	id(id)
{
	lastUsedId = max(id, lastUsedId);
}

string to_string(const IdType& idType) {
	ostringstream o;
	o << (unsigned)idType;
	return o.str();
}
