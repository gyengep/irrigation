#pragma once
#include <gmock/gmock.h>
#include "Model/IrrigationDocument.h"



class MockIrrigationDocumentSaver : public IrrigationDocument::Saver {
public:
	MOCK_METHOD1(save, void(const IrrigationDocument&));
};
