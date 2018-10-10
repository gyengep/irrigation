#pragma once
#include <string>
#include <memory>
#include "DtoReaderWriterFactory.h"


namespace pugi {
	class xml_document;
	class xml_node;
};


class XmlWriter : public DtoWriter {
	static std::string toString(const pugi::xml_document* doc, bool humanReadable);

	const bool humanReadable;

	void saveDocument(pugi::xml_node* parent, const DocumentDTO& document);
	void saveProgram(pugi::xml_node* parent, const ProgramDTO& program);
	void saveRunTime(pugi::xml_node* parent, const RunTimeDTO& runTime);
	void saveStartTime(pugi::xml_node* parent, const StartTimeDTO& startTime);
	void saveScheduler(pugi::xml_node* parent, const SpecifiedSchedulerDTO& scheduler);

public:
	XmlWriter(bool humanReadable = true);
	virtual ~XmlWriter();

	virtual std::string save(const DocumentDTO& document) override;
	virtual std::string save(const ProgramDTO& program) override;
	virtual std::string save(const RunTimeDTO& runTime) override;
	virtual std::string save(const StartTimeDTO& startTime) override;
	virtual std::string save(const SpecifiedSchedulerDTO& scheduler) override;
};
