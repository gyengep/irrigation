#pragma once
#include <gmock/gmock.h>
#include "Logic/ProgramContainer.h"


class MockProgramContainer : public ProgramContainer {
public:

	MockProgramContainer() {
		ON_CALL(*this, insert(testing::_, testing::_)).WillByDefault(testing::Invoke(this, &MockProgramContainer::insertToProgramContainer));
		ON_CALL(*this, erase(testing::_)).WillByDefault(testing::Invoke(this, &MockProgramContainer::eraseFromStartTimeContainer));
	}

	value_type& insertToProgramContainer(const key_type& key, const mapped_type& value) {
		return ProgramContainer::insert(key, value);
	}

	void eraseFromStartTimeContainer(const key_type& key) {
		ProgramContainer::erase(key);
	}

	MOCK_METHOD2(insert, value_type&(const key_type& key, const mapped_type& value));
	MOCK_METHOD1(erase, void(const key_type& key));
};
