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
	std::unique_ptr<pugi::xml_document> doc;

	void saveDocument(pugi::xml_node& parent, const DocumentDTO& document);
	void saveProgram(pugi::xml_node& parent, const ProgramDTO& program);
	void saveRunTime(pugi::xml_node& parent, const RunTimeDTO& runTime);
	void saveStartTime(pugi::xml_node& parent, const StartTimeDTO& startTime);
	void saveScheduler(pugi::xml_node& parent, const SpecifiedSchedulerDTO& scheduler);

public:
	XmlWriter();
	virtual ~XmlWriter();

	virtual std::string toString(bool humanReadable = true) const;

	virtual void save(const DocumentDTO& document);
	virtual void save(const ProgramDTO& program);
	virtual void save(const RunTimeDTO& runTime);
	virtual void save(const StartTimeDTO& startTime);
	virtual void save(const SpecifiedSchedulerDTO& scheduler);
};
