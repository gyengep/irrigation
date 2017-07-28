#pragma once

#include "Model/IrrigationDocument.h"
#include "Logic/Containers.h"

/*
class ProgramContainerMock {
public:
	typedef ProgramContainer::value_type value_type;

	MOCK_METHOD1(insert, IdType(const value_type& newItem));
	MOCK_METHOD1(erase, void(IdType id));
	MOCK_METHOD2(move, void(IdType id, size_t newPosition));
};

class IrrigationDocumentMock : public IrrigationDocument<ProgramContainerMock> {
public:
	IrrigationDocumentMock() : IrrigationDocument<ProgramContainerMock>() {}
};


class ProgramTest : public ::testing::Test {
protected:

	IrrigationDocumentMock document;

    virtual void SetUp();
    virtual void TearDown();
};
*/
