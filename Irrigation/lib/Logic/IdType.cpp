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

string IdType::toString() const {
	ostringstream o;
	o << "IdType{id=" << id << "}";
	return o.str();
}
