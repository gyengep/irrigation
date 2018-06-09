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


class XmlReaderException : public std::runtime_error {
public:
	XmlReaderException(const char* what) : std::runtime_error(what) {}
	XmlReaderException(const std::string& what) : std::runtime_error(what) {}
};

class XmlParsingError : public XmlReaderException {
public:
	XmlParsingError(const char* what) :
		XmlReaderException(std::string("XML parsing error: ") + what) {}
};

class RequiredTagMissing : public XmlReaderException {
public:
	RequiredTagMissing(const char* what) :
		XmlReaderException(std::string("Required tag missing: ") + what) {}
};

class BadSchedulerType : public XmlReaderException {
public:
	BadSchedulerType(const char* what) :
		XmlReaderException(std::string("Scheduler type has to be: ") + what) {}
};


class XmlReader {
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	void loadDocument(const pugi::xml_node& node, DocumentDTO& document) const;
	void loadProgram(const pugi::xml_node& node, ProgramDTO& program) const;
	void loadRunTime(const pugi::xml_node& node, RunTimeDTO& runTime) const;
	void loadStartTime(const pugi::xml_node& node, StartTimeDTO& startTime) const;
	void loadScheduler(const pugi::xml_node& node, SpecifiedSchedulerDTO& scheduler) const;

public:
	XmlReader();
	virtual ~XmlReader();

	virtual void load(DocumentDTO& document, const std::string& text) const;
	virtual void load(ProgramDTO& program, const std::string& text) const;
	virtual void load(RunTimeDTO& runTime, const std::string& text) const;
	virtual void load(StartTimeDTO& startTime, const std::string& text) const;
	virtual void load(SpecifiedSchedulerDTO& scheduler, const std::string& text) const;
};
