#include "IdType.h"
#include "Exceptions/IllegalArgumentException.h"
#include <algorithm>
#include <limits>
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

IdType IdType::from_string(const string& id) {

	if (id.find_first_of("0123456789") != 0) {
		throw IllegalArgumentException("Invalid id: \"" + id + "\"");
	}

	char* end = NULL;
	unsigned long result = strtoul(id.c_str(), &end, 10);

	if (*end != '\0') {
		throw IllegalArgumentException("Invalid id: \"" + id + "\"");
	}

	if (result > numeric_limits<unsigned>::max()) {
		throw IllegalArgumentException("Id is too large: \"" + id + "\"");
	}

	return result;
}
