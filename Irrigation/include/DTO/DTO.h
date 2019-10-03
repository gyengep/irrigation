#pragma once


#define CAT(a,b) a##b
#define TEXTIFY(A) #A

#define DECLARE_DTO_VALUE_COPY(CLASS, TYPE, NAME) 			\
private:													\
	std::unique_ptr<const TYPE> NAME;						\
public:														\
	bool CAT(has, NAME)() const;							\
	const TYPE& CAT(get, NAME)() const;						\
	CLASS& CAT(set, NAME)(const TYPE& NAME);				\


#define IMPLEMENT_DTO_VALUE_COPY(CLASS, TYPE, NAME) 		\
bool CLASS::CAT(has, NAME)() const {						\
	return (NAME.get() != nullptr);							\
}															\
CLASS& CLASS::CAT(set, NAME)(const TYPE& value) {			\
	this->NAME.reset(new TYPE(value));						\
	return *this;											\
}															\
const TYPE& CLASS::CAT(get, NAME)() const {					\
	if (!CAT(has, NAME)()) {								\
		throw logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  \
	}														\
	return *NAME.get();										\
}



#define DECLARE_DTO_VALUE_MOVE(CLASS, TYPE, NAME) 			\
private:													\
	std::unique_ptr<const TYPE> NAME;						\
public:														\
	bool CAT(has, NAME)() const;							\
	const TYPE& CAT(get, NAME)() const;						\
	CLASS& CAT(set, NAME)(TYPE&& NAME);						\


#define IMPLEMENT_DTO_VALUE_MOVE(CLASS, TYPE, NAME) 		\
bool CLASS::CAT(has, NAME)() const {						\
	return (NAME.get() != nullptr);							\
}															\
CLASS& CLASS::CAT(set, NAME)(TYPE&& value) {				\
	this->NAME.reset(new TYPE(std::move(value)));			\
	return *this;											\
}															\
const TYPE& CLASS::CAT(get, NAME)() const {					\
	if (!CAT(has, NAME)()) {								\
		throw logic_error(TEXTIFY(CLASS) "::get" TEXTIFY(NAME) "(): !has" TEXTIFY(NAME) "()");  \
	}														\
	return *NAME.get();										\
}
