#pragma once

#include <teng.h>

#include "Logic/Containers.h"

class IrrigationDocument;


class TemplateEngine {
	const IrrigationDocument* const document;
	Teng::Teng_t teng;
	std::string result;

	void onIterate(Teng::FragmentList_t* programList, IdType id, LockedProgram program);
	void initData(Teng::Fragment_t& root);

public:
	TemplateEngine(IrrigationDocument* const document);
	~TemplateEngine();

	const std::string& generate(const std::string& source);
};
