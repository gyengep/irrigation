#pragma once
#include <gmock/gmock.h>
#include "Logic/ProgramContainer.h"


class MockProgramContainer : public ProgramContainer {
public:
	MOCK_METHOD2(insert, value_type&(const key_type& key, const mapped_type& value));
	MOCK_METHOD1(erase, void(const key_type& key));

	MOCK_CONST_METHOD0(begin, const_iterator());
	MOCK_CONST_METHOD0(end, const_iterator());
	MOCK_CONST_METHOD0(size, size_t());
	MOCK_CONST_METHOD0(empty, bool());

	MOCK_CONST_METHOD1(at, const_mapped_type(const key_type& key));
	MOCK_METHOD1(at, mapped_type(const key_type& key));

	MOCK_CONST_METHOD0(toProgramDtoList, ProgramDtoList());
	MOCK_METHOD1(updateFromProgramDtoList, void(const ProgramDtoList& programDtoList));
	MOCK_METHOD1(createFromProgramDto, value_type&(const ProgramDto& programDto));

	MOCK_CONST_METHOD0(toString, std::string());
};
