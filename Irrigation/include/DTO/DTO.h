#pragma once


#define CAT(a,b) a##b
#define TEXTIFY(A) #A
#define GET_FUNCTION(NAME) get##NAME
#define HAS_FUNCTION(NAME) has##NAME
#define SET_FUNCTION(NAME) set##NAME

#define DECLARE_DTO_VALUE_COPY(CLASS, TYPE, NAME) 			\
private:													\
	std::unique_ptr<const TYPE> NAME;						\
public:														\
	bool HAS_FUNCTION(NAME)() const;						\
	const TYPE& GET_FUNCTION(NAME)() const;					\
	CLASS& SET_FUNCTION(NAME)(const TYPE& NAME);			\


#define IMPLEMENT_DTO_VALUE_COPY(CLASS, TYPE, NAME) 		\
bool CLASS::HAS_FUNCTION(NAME)() const {					\
	return (NAME.get() != nullptr);							\
}															\
CLASS& CLASS::SET_FUNCTION(NAME)(const TYPE& value) {		\
	this->NAME.reset(new TYPE(value));						\
	return *this;											\
}															\
const TYPE& CLASS::GET_FUNCTION(NAME)() const {				\
	if (!HAS_FUNCTION(NAME)()) {							\
		throw logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  \
	}														\
	return *NAME.get();										\
}



#define DECLARE_DTO_VALUE_MOVE(CLASS, TYPE, NAME) 			\
private:													\
	std::unique_ptr<const TYPE> NAME;						\
public:														\
	bool HAS_FUNCTION(NAME)() const;						\
	const TYPE& GET_FUNCTION(NAME)() const;					\
	CLASS& SET_FUNCTION(NAME)(TYPE&& NAME);					\


#define IMPLEMENT_DTO_VALUE_MOVE(CLASS, TYPE, NAME) 		\
bool CLASS::HAS_FUNCTION(NAME)() const {					\
	return (NAME.get() != nullptr);							\
}															\
CLASS& CLASS::SET_FUNCTION(NAME)(TYPE&& value) {			\
	this->NAME.reset(new TYPE(std::move(value)));			\
	return *this;											\
}															\
const TYPE& CLASS::GET_FUNCTION(NAME)() const {				\
	if (!HAS_FUNCTION(NAME)()) {							\
		throw logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  \
	}														\
	return *NAME.get();										\
}
