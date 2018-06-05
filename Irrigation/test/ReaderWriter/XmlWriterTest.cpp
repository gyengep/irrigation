#include "XmlWriterTest.h"
#include <algorithm>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include "ReaderWriter/DocumentDTO.h"
#include "ReaderWriter/ProgramDTO.h"
#include "ReaderWriter/RunTimeDTO.h"
#include "ReaderWriter/StartTimeDTO.h"
#include "ReaderWriter/SchedulersDTO.h"
#include "ReaderWriter/XmlWriter.h"

using namespace std;

void check_xml(const XmlWriter& writer, const string& expected) {
	string actual = writer.toString(false);

	size_t pos = actual.find('>');
	if (string::npos == pos) {
		throw invalid_argument("check_xml");
	}

	actual.erase(0, pos + 1);
	EXPECT_EQ(expected, actual);
}

SpecifiedSchedulerDTO* XmlWriterTest::createSpecifiedSchedulerDTO(const char* value1, const char* value2, const char* value3) {
	list<string>* values = new list<string>();

	if (value1 != nullptr) {
		values->push_back(value1);
	}

	if (value2 != nullptr) {
		values->push_back(value2);
	}

	if (value3 != nullptr) {
		values->push_back(value3);
	}

	return new SpecifiedSchedulerDTO(values);
}

list<RunTimeDTO>* XmlWriterTest::createRunTimeDTOList(const char* value1, const char* value2, const char* value3) {
	list<RunTimeDTO>* result = new list<RunTimeDTO>();

	if (value1 != nullptr) {
		result->push_back(RunTimeDTO(value1));
	}

	if (value2 != nullptr) {
		result->push_back(RunTimeDTO(value2));
	}

	if (value3 != nullptr) {
		result->push_back(RunTimeDTO(value3));
	}

	return result;
}

list<StartTimeDTO>* XmlWriterTest::createStartTimeDTOList(const char* value1, const char* value2, const char* value3) {
	list<StartTimeDTO>* result = new list<StartTimeDTO>();

	if (value1 != nullptr) {
		result->push_back(StartTimeDTO(value1));
	}

	if (value2 != nullptr) {
		result->push_back(StartTimeDTO(value2));
	}

	if (value3 != nullptr) {
		result->push_back(StartTimeDTO(value3));
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(XmlWriterTest, writeRunTimeConstruct) {
	RunTimeDTO runTime("20");

	writer.save(runTime);

	check_xml(writer, "<runtime>20</runtime>");
}

TEST_F(XmlWriterTest, writeRunTime) {
	RunTimeDTO runTime;
	runTime.setValue("20");

	writer.save(runTime);

	check_xml(writer, "<runtime>20</runtime>");
}

TEST_F(XmlWriterTest, writeRunTimeEmpty) {
	RunTimeDTO runTime;

	writer.save(runTime);

	check_xml(writer, "");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(XmlWriterTest, writeStartTimeConstruct) {
	StartTimeDTO startTime("50");

	writer.save(startTime);

	check_xml(writer, "<starttime>50</starttime>");
}

TEST_F(XmlWriterTest, writeStartTime) {
	StartTimeDTO startTime;
	startTime.setValue("50");

	writer.save(startTime);

	check_xml(writer, "<starttime>50</starttime>");
}

TEST_F(XmlWriterTest, writeStartTimeEmpty) {
	StartTimeDTO startTime;

	writer.save(startTime);

	check_xml(writer, "");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(XmlWriterTest, writeSpecifiedScedulerConstruct) {
	SpecifiedSchedulerDTO scheduler(
		new list<string>({
			"true",
			"false",
			"false",
			"true"
		}));

	writer.save(scheduler);

	const string expected =
			"<scheduler type=\"specified\">"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
			"</scheduler>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeSpecifiedSceduler) {
	SpecifiedSchedulerDTO scheduler;
	scheduler.setValues(
		new list<string>({
			"true",
			"false",
			"false",
			"true"
		}));

	writer.save(scheduler);

	const string expected =
			"<scheduler type=\"specified\">"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
			"</scheduler>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeSpecifiedScedulerEmpty) {
	SpecifiedSchedulerDTO scheduler;

	writer.save(scheduler);

	check_xml(writer, "");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(XmlWriterTest, writeProgramEmpty) {
	ProgramDTO program;

	writer.save(program);

	check_xml(writer, "<program/>");
}

TEST_F(XmlWriterTest, writeProgramName) {
	ProgramDTO program;
	program.setName("abcdefg");

	writer.save(program);

	const string expected =
			"<program>"
				"<name>abcdefg</name>"
			"</program>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramSchedulerType) {
	ProgramDTO program;
	program.setSchedulerType("specified");

	writer.save(program);

	const string expected =
			"<program>"
				"<schedulertype>specified</schedulertype>"
			"</program>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramSpecifiedScheduler) {
	ProgramDTO program;
	program.setSpecifiedScheduler(createSpecifiedSchedulerDTO("true", "false"));

	writer.save(program);

	const string expected =
			"<program>"
				"<schedulers>"
					"<scheduler type=\"specified\">"
						"<day>true</day>"
						"<day>false</day>"
					"</scheduler>"
				"</schedulers>"
			"</program>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramRunTimes) {
	ProgramDTO program;
	program.setRunTimes(createRunTimeDTOList("20", "10"));

	writer.save(program);

	const string expected =
			"<program>"
				"<runtimes>"
					"<runtime>20</runtime>"
					"<runtime>10</runtime>"
				"</runtimes>"
			"</program>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramStartTimes) {
	ProgramDTO program;
	program.setStartTimes(createStartTimeDTOList("50", "30", "100"));

	writer.save(program);

	const string expected =
			"<program>"
				"<starttimes>"
					"<starttime>50</starttime>"
					"<starttime>30</starttime>"
					"<starttime>100</starttime>"
				"</starttimes>"
			"</program>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramAll) {
	ProgramDTO program;
	program.setName("abcdefg");
	program.setSchedulerType("specified");
	program.setSpecifiedScheduler(createSpecifiedSchedulerDTO("true", "false"));
	program.setRunTimes(createRunTimeDTOList("20", "10"));
	program.setStartTimes(createStartTimeDTOList("50", "30", "100"));

	writer.save(program);

	const string expected =
			"<program>"
				"<name>abcdefg</name>"
				"<schedulertype>specified</schedulertype>"
				"<schedulers>"
					"<scheduler type=\"specified\">"
						"<day>true</day>"
						"<day>false</day>"
					"</scheduler>"
				"</schedulers>"
				"<runtimes>"
					"<runtime>20</runtime>"
					"<runtime>10</runtime>"
				"</runtimes>"
				"<starttimes>"
					"<starttime>50</starttime>"
					"<starttime>30</starttime>"
					"<starttime>100</starttime>"
				"</starttimes>"
			"</program>";


	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeProgramAllConstructor) {
	ProgramDTO program( "abcdefg", "specified",
		createSpecifiedSchedulerDTO("true", "false"),
		createRunTimeDTOList("20", "10"),
		createStartTimeDTOList("50", "30", "100")
		);

	writer.save(program);

	const string expected =
			"<program>"
				"<name>abcdefg</name>"
				"<schedulertype>specified</schedulertype>"
				"<schedulers>"
					"<scheduler type=\"specified\">"
						"<day>true</day>"
						"<day>false</day>"
					"</scheduler>"
				"</schedulers>"
				"<runtimes>"
					"<runtime>20</runtime>"
					"<runtime>10</runtime>"
				"</runtimes>"
				"<starttimes>"
					"<starttime>50</starttime>"
					"<starttime>30</starttime>"
					"<starttime>100</starttime>"
				"</starttimes>"
			"</program>";

	check_xml(writer, expected);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(XmlWriterTest, writeDocument) {
	list<ProgramDTO>* programs = new list<ProgramDTO>();

	programs->push_back(
		ProgramDTO( "abcdefg", "specified",
			createSpecifiedSchedulerDTO("true", "false"),
			createRunTimeDTOList("20", "10"),
			createStartTimeDTOList("50", "30", "100")
		));

	programs->push_back(
		ProgramDTO( "program2", "unknown",
			createSpecifiedSchedulerDTO("true", "true", "false"),
			createRunTimeDTOList("100", "99", "102"),
			createStartTimeDTOList("48", "23")
		));


	DocumentDTO document;
	document.setPrograms(programs);

	writer.save(document);

	const string expected =
			"<irrigation>"
				"<programs>"
					"<program>"
						"<name>abcdefg</name>"
						"<schedulertype>specified</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<day>true</day>"
								"<day>false</day>"
							"</scheduler>"
						"</schedulers>"
						"<runtimes>"
							"<runtime>20</runtime>"
							"<runtime>10</runtime>"
						"</runtimes>"
						"<starttimes>"
							"<starttime>50</starttime>"
							"<starttime>30</starttime>"
							"<starttime>100</starttime>"
						"</starttimes>"
					"</program>"
					"<program>"
						"<name>program2</name>"
						"<schedulertype>unknown</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<day>true</day>"
								"<day>true</day>"
								"<day>false</day>"
							"</scheduler>"
						"</schedulers>"
						"<runtimes>"
							"<runtime>100</runtime>"
							"<runtime>99</runtime>"
							"<runtime>102</runtime>"
						"</runtimes>"
						"<starttimes>"
							"<starttime>48</starttime>"
							"<starttime>23</starttime>"
						"</starttimes>"
					"</program>"
				"</programs>"
			"</irrigation>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeDocumentConstructor) {
	list<ProgramDTO>* programs = new list<ProgramDTO>();

	programs->push_back(
		ProgramDTO( "abcdefg", "specified",
			createSpecifiedSchedulerDTO("true", "false"),
			createRunTimeDTOList("20", "10"),
			createStartTimeDTOList("50", "30", "100")
		));

	programs->push_back(
		ProgramDTO( "program2", "unknown",
			createSpecifiedSchedulerDTO("true", "true", "false"),
			createRunTimeDTOList("100", "99", "102"),
			createStartTimeDTOList("48", "23")
		));


	DocumentDTO document(programs);

	writer.save(document);

	const string expected =
			"<irrigation>"
				"<programs>"
					"<program>"
						"<name>abcdefg</name>"
						"<schedulertype>specified</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<day>true</day>"
								"<day>false</day>"
							"</scheduler>"
						"</schedulers>"
						"<runtimes>"
							"<runtime>20</runtime>"
							"<runtime>10</runtime>"
						"</runtimes>"
						"<starttimes>"
							"<starttime>50</starttime>"
							"<starttime>30</starttime>"
							"<starttime>100</starttime>"
						"</starttimes>"
					"</program>"
					"<program>"
						"<name>program2</name>"
						"<schedulertype>unknown</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<day>true</day>"
								"<day>true</day>"
								"<day>false</day>"
							"</scheduler>"
						"</schedulers>"
						"<runtimes>"
							"<runtime>100</runtime>"
							"<runtime>99</runtime>"
							"<runtime>102</runtime>"
						"</runtimes>"
						"<starttimes>"
							"<starttime>48</starttime>"
							"<starttime>23</starttime>"
						"</starttimes>"
					"</program>"
				"</programs>"
			"</irrigation>";

	check_xml(writer, expected);
}

TEST_F(XmlWriterTest, writeDocumentEmpty) {
	DocumentDTO document;

	writer.save(document);

	check_xml(writer, "<irrigation/>");
}

