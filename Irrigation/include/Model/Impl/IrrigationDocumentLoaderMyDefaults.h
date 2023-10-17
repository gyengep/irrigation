#pragma once
#include "Model/IrrigationDocument.h"



class IrrigationDocumentLoaderMyDefaults : public IrrigationDocument::Loader {
public:
	IrrigationDocumentLoaderMyDefaults();
	virtual ~IrrigationDocumentLoaderMyDefaults();

	virtual bool load(IrrigationDocument& document) override;
};
