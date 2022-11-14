#pragma once
#include <gmock/gmock.h>
#include "Logic/RunTimeContainer.h"


class MockRunTimeContainer : public RunTimeContainer {
public:
	MOCK_CONST_METHOD0(begin, const_iterator());
	MOCK_CONST_METHOD0(end, const_iterator());
	MOCK_CONST_METHOD0(size, size_t());

	MOCK_CONST_METHOD1(at, const_mapped_type(const key_type& key));
	MOCK_METHOD1(at, mapped_type(const key_type& key));

	MOCK_CONST_METHOD0(toRunTimeDtoList, std::list<RunTimeDTO>());
	MOCK_METHOD1(updateFromRunTimeDtoList, void(const std::list<RunTimeDTO>&));

	MOCK_CONST_METHOD0(toString, std::string());
	MOCK_CONST_METHOD0(toDurationList, DurationList());
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeContainerFactory : public RunTimeContainerFactory {
public:
	virtual ~MockRunTimeContainerFactory() = default;
	virtual RunTimeContainerPtr create() const override {
		return std::make_shared<MockRunTimeContainer>();
	}
};
