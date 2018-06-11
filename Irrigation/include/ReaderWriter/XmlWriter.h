#pragma once
#include <string>
#include <memory>


class DocumentDTO;
class ProgramDTO;
class RunTimeDTO;
class StartTimeDTO;
class SpecifiedSchedulerDTO;

namespace pugi {
	class xml_document;
	class xml_node;
};


class XmlWriter {
	static std::string toString(const pugi::xml_document* doc, bool humanReadable);

	void saveDocument(pugi::xml_node* parent, const DocumentDTO& document);
	void saveProgram(pugi::xml_node* parent, const ProgramDTO& program);
	void saveRunTime(pugi::xml_node* parent, const RunTimeDTO& runTime);
	void saveStartTime(pugi::xml_node* parent, const StartTimeDTO& startTime);
	void saveScheduler(pugi::xml_node* parent, const SpecifiedSchedulerDTO& scheduler);

public:
	XmlWriter();
	virtual ~XmlWriter();

	virtual std::string save(const DocumentDTO& document, bool humanReadable = true);
	virtual std::string save(const ProgramDTO& program, bool humanReadable = true);
	virtual std::string save(const RunTimeDTO& runTime, bool humanReadable = true);
	virtual std::string save(const StartTimeDTO& startTime, bool humanReadable = true);
	virtual std::string save(const SpecifiedSchedulerDTO& scheduler, bool humanReadable = true);
};
