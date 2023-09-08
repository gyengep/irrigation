#pragma once
#include <gmock/gmock.h>
#include "Model/IrrigationDocument.h"



class MockIrrigationDocumentLoader : public IrrigationDocument::Loader {
public:
	MOCK_METHOD1(load, bool(IrrigationDocument&));
};
