#pragma once
#include <gmock/gmock.h>
#include "Logic/StartTimeContainer.h"


class MockStartTimeContainer : public StartTimeContainer {
public:
	MOCK_CONST_METHOD0(begin, const_iterator());
	MOCK_CONST_METHOD0(end, const_iterator());
	MOCK_CONST_METHOD0(size, size_t());
	MOCK_CONST_METHOD0(empty, bool());

	MOCK_METHOD2(insert, value_type&(const key_type& key, const mapped_type& value));
	MOCK_METHOD1(erase, void(const key_type& key));
	MOCK_METHOD0(sort, void());

	MOCK_CONST_METHOD1(at, const_mapped_type(const key_type& key));
	MOCK_METHOD1(at, mapped_type(const key_type& key));

	MOCK_CONST_METHOD0(toStartTimeDtoList, std::list<StartTimeDTO>());
	MOCK_METHOD2(updateFromStartTimeDtoList, void(const std::shared_ptr<StartTimeFactory>& startTimeFactory, const std::list<StartTimeDTO>& startTimeDtoList));

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockStartTimeContainerFactory : public StartTimeContainerFactory {
public:
	virtual ~MockStartTimeContainerFactory() = default;
	virtual StartTimeContainerPtr create() const override {
		return std::make_shared<MockStartTimeContainer>();
	}
};
