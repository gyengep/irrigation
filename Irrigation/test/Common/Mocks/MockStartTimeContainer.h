#pragma once
#include <gmock/gmock.h>
#include "Logic/StartTimeContainer.h"


class MockStartTimeContainer : public StartTimeContainer {
public:

	MockStartTimeContainer() {
		ON_CALL(*this, insert(testing::_, testing::_)).WillByDefault(testing::Invoke(this, &MockStartTimeContainer::insertToStartTimeContainer));
		ON_CALL(*this, erase(testing::_)).WillByDefault(testing::Invoke(this, &MockStartTimeContainer::eraseFromStartTimeContainer));
	}

	value_type& insertToStartTimeContainer(const key_type& key, const mapped_type& value) {
		return StartTimeContainer::insert(key, value);
	}

	void eraseFromStartTimeContainer(const key_type& key) {
		StartTimeContainer::erase(key);
	}

	MOCK_METHOD2(insert, value_type&(const key_type& key, const mapped_type& value));
	MOCK_METHOD1(erase, void(const key_type& key));
};
