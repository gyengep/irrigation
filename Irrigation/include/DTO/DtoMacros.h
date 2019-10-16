#pragma once
#include <list>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>


#define CAT(a,b) a##b
#define TEXTIFY(A) #A
#define GET_FUNCTION(NAME) get##NAME
#define HAS_FUNCTION(NAME) has##NAME
#define SET_FUNCTION(NAME) set##NAME

///////////////////////////////////////////////////////////////////////////////

#define CREATE_DTO_CLASS(CLASS)																			\
class CLASS {																							\
public:																									\
	CLASS() = default;																					\
	CLASS(CLASS&& other) = default;																		\
																										\
	CLASS& operator= (CLASS&&) = default;																\
	CLASS& operator= (const CLASS&) = delete;															\
	friend std::ostream& operator<<(std::ostream& os, const CLASS&);

///////////////////////////////////////////////////////////////////////////////

#define DTO_DECLARE_VALUE_COPY(CLASS, TYPE, NAME) 														\
private:																								\
	std::unique_ptr<const TYPE> NAME;																	\
public:																									\
bool HAS_FUNCTION(NAME)() const {																		\
	return (NAME.get() != nullptr);																		\
}																										\
CLASS& SET_FUNCTION(NAME)(const TYPE& value) {															\
	this->NAME.reset(new TYPE(value));																	\
	return *this;																						\
}																										\
const TYPE& GET_FUNCTION(NAME)() const {																\
	if (!HAS_FUNCTION(NAME)()) {																		\
		throw std::logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  	\
	}																									\
	return *NAME.get();																					\
}

#define DTO_DECLARE_VALUE_MOVE(CLASS, TYPE, NAME) 														\
private:																								\
	std::unique_ptr<const TYPE> NAME;																	\
public:																									\
bool HAS_FUNCTION(NAME)() const {																		\
	return (NAME.get() != nullptr);																		\
}																										\
CLASS& SET_FUNCTION(NAME)(TYPE&& value) {																\
	this->NAME.reset(new TYPE(std::move(value)));														\
	return *this;																						\
}																										\
const TYPE& GET_FUNCTION(NAME)() const {																\
	if (!HAS_FUNCTION(NAME)()) {																		\
		throw std::logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  	\
	}																									\
	return *NAME.get();																					\
}

///////////////////////////////////////////////////////////////////////////////

#define DTO_COPY_CTOR_COPY(CLASS, TYPE, NAME)															\
	if (other.HAS_FUNCTION(NAME)()) {																	\
		SET_FUNCTION(NAME)(other.GET_FUNCTION(NAME)());													\
	}

#define DTO_COPY_CTOR_MOVE(CLASS, TYPE, NAME)															\
	if (other.HAS_FUNCTION(NAME)()) {																	\
		SET_FUNCTION(NAME)(std::move(TYPE(other.GET_FUNCTION(NAME)())));								\
	}

#define IMPLEMENT_COPY_CTOR(CLASS, MEMBERS)																\
	CLASS(const CLASS& other) {																			\
		MEMBERS;																						\
	}

///////////////////////////////////////////////////////////////////////////////

#define DTO_INIT_VALUE_COPY(CLASS, TYPE, NAME)															\
	SET_FUNCTION(NAME)(NAME)

#define DTO_INIT_VALUE_MOVE(CLASS, TYPE, NAME)															\
	SET_FUNCTION(NAME)(std::move(TYPE(NAME)))

///////////////////////////////////////////////////////////////////////////////

#define DTO_EQUALS_OPERATOR(CLASS, TYPE, NAME)															\
	if (false == equalsPtr(NAME.get(), other.NAME.get())) {												\
		return false;																					\
	}

#define IMPLEMENT_EQUALS_OPERATOR(CLASS, MEMBERS)														\
	bool operator== (const CLASS& other) const {														\
		MEMBERS;																						\
		return true;																					\
	}

///////////////////////////////////////////////////////////////////////////////

#define DTO_OSS_OPERATOR(CLASS, TYPE, NAME)																\
	{																									\
		std::ostringstream oss;																			\
		if (dto.NAME.get()) {																			\
			oss << dto_to_string(*dto.NAME.get());														\
		} else {																						\
			oss << "<nullptr>";																			\
		}																								\
		members.push_back(std::make_pair(TEXTIFY(NAME), oss.str()));									\
	}


#define IMPLEMENT_OSS_OPERATOR(CLASS, MEMBERS)															\
inline std::ostream& operator<<(std::ostream& os, const CLASS& dto) {									\
	std::list<std::pair<std::string, std::string>> members;												\
																										\
	MEMBERS;																							\
																										\
	os << TEXTIFY(CLASS) << "{";																		\
	for (auto it = members.begin(); it != members.end(); ++it) {										\
		if (it != members.begin()) {																	\
			os << ", ";																					\
		}																								\
		os << it->first << "=" << it->second;															\
	}																									\
	os << "}";																							\
																										\
	return os;																							\
}

///////////////////////////////////////////////////////////////////////////////

template<typename TYPE>
bool equalsPtr(const TYPE* p1, const TYPE* p2) {
	if (p1 == nullptr && p2 == nullptr) {
		return true;
	}

	if (p1 != nullptr && p2 != nullptr) {
		return (*p1 == *p2);
	}

	return false;
}

template <typename TYPE>
std::string dto_to_string(const TYPE& a) {
	std::ostringstream oss;
	oss << a;
	return oss.str();
}

template <>
inline std::string dto_to_string<std::string>(const std::string& text) {
	return '\"' + text + '\"';
}

template<typename TYPE>
std::string dto_to_string(const std::list<TYPE>& list) {
	std::ostringstream oss;
	oss << "[";
	for(auto it = list.begin(); it != list.end(); ++it) {
		if (it != list.begin()) {
			oss << ", ";
		}
    	oss << *it;
	}
	oss << "]";

	return oss.str();
}
