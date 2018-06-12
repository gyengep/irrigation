#include "IdType.h"
#include <algorithm>

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
