#pragma once

#include <teng.h>

#include "Logic/Containers.h"
#include "Model/IrrigationDocument.h"


class TemplateEngine {
	const IrrigationDocument* const document;
	std::string generated;

	void onIterate(Teng::FragmentList_t* programList, IdType id, LockedProgram program);
	void addDatas(Teng::Fragment_t& root);
	void addParameters(Teng::Fragment_t& root, const std::map<std::string, std::string>& parameters);

public:
	TemplateEngine(IrrigationDocument* const document);
	~TemplateEngine();

	const std::string& generate(const std::string& source, const std::map<std::string, std::string>& parameters);
};
