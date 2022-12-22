#include "XmlWriter.h"
#include "pugixml.hpp"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace pugi;



XmlWriter::XmlWriter(bool humanReadable) : humanReadable(humanReadable) {
}

std::string XmlWriter::getContentType() const {
	return "application/xml";
}

string XmlWriter::toString(const xml_document* doc, bool humanReadable) {
	ostringstream o;
	if (humanReadable) {
		doc->save(o);
	} else {
		doc->save(o, "\t", format_raw);
	}
	return o.str();
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::prependStyleSheet(pugi::xml_node* parent, const std::string& styleSheet) {
	xml_node processingInstructionNode = parent->prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value(std::string("type=\"text/xsl\" href=\"" + styleSheet + "\"").c_str());
}

void XmlWriter::saveDocument(xml_node* parent, const DocumentDTO& document) {
	xml_node node = parent->append_child("irrigation");

	if (document.hasPrograms()) {
		saveProgramList(&node, document.getPrograms(), true);
	}
}

void XmlWriter::saveProgramList(pugi::xml_node* parent, const list<ProgramDTO>& programs, bool includeContainers) {
	xml_node programListNode = parent->append_child("programs");

	for (auto& program : programs) {
		saveProgram(&programListNode, program, includeContainers);
	}
}

void XmlWriter::saveProgram(xml_node* parent, const ProgramDTO& program, bool includeContainers) {
	xml_node node = parent->append_child("program");

	if (program.hasId()) {
		node.append_attribute("id").set_value(program.getId());
	}

	if (program.hasName()) {
		node.append_child("name").text().set(program.getName().c_str());
	}

	if (program.hasEnabled()) {
		node.append_child("enabled").text().set(program.getEnabled());
	}

	if (program.hasAdjustment()) {
		node.append_child("adjustment").text().set(program.getAdjustment());
	}

	if (program.hasSchedulerType()) {
		node.append_child("schedulertype").text().set(program.getSchedulerType().c_str());
	}

	if (includeContainers) {
		if (program.hasSchedulers()) {
			saveSchedulers(&node, program.getSchedulers());
		}

		if (program.hasRunTimes()) {
			saveRunTimeList(&node, program.getRunTimes());
		}

		if (program.hasStartTimes()) {
			saveStartTimeList(&node, program.getStartTimes());
		}
	}
}

void XmlWriter::saveRunTimeList(pugi::xml_node* parent, const list<RunTimeDTO>& runTimes) {
	xml_node runTimeListNode = parent->append_child("runtimes");

	for (auto& runTime : runTimes) {
		saveRunTime(&runTimeListNode, runTime);
	}
}

void XmlWriter::saveRunTime(xml_node* parent, const RunTimeDTO& runTime) {
	xml_node node = parent->append_child("runtime");

	if (runTime.hasMinutes()) {
		node.append_child("minute").text().set(runTime.getMinutes());
	}

	if (runTime.hasSeconds()) {
		node.append_child("second").text().set(runTime.getSeconds());
	}

	if (runTime.hasId()) {
		node.append_attribute("id").set_value(runTime.getId());
	}
}

void XmlWriter::saveStartTimeList(pugi::xml_node* parent, const list<StartTimeDTO>& startTimes) {
	xml_node startTimeListNode = parent->append_child("starttimes");

	for (auto& startTime : startTimes) {
		saveStartTime(&startTimeListNode, startTime);
	}
}

void XmlWriter::saveStartTime(xml_node* parent, const StartTimeDTO& startTime) {
	xml_node node = parent->append_child("starttime");

	if (startTime.hasHours()) {
		node.append_child("hour").text().set(startTime.getHours());
	}

	if (startTime.hasMinutes()) {
		node.append_child("minute").text().set(startTime.getMinutes());
	}

	if (startTime.hasId()) {
		node.append_attribute("id").set_value(startTime.getId());
	}
}

void XmlWriter::saveSchedulers(xml_node* parent, const SchedulersDTO& schedulers) {
	xml_node schedulersListNode = parent->append_child("schedulers");

	if (schedulers.hasEveryDayScheduler()) {
		const EveryDaySchedulerDTO& everyDayScheduler = schedulers.getEveryDayScheduler();
		saveEveryDayScheduler(&schedulersListNode, everyDayScheduler);
	}

	if (schedulers.hasHotWeatherScheduler()) {
		const HotWeatherSchedulerDTO& hotWeatherScheduler = schedulers.getHotWeatherScheduler();
		saveHotWeatherScheduler(&schedulersListNode, hotWeatherScheduler);
	}

	if (schedulers.hasTemperatureDependentScheduler()) {
		const TemperatureDependentSchedulerDTO& temperatureDependentScheduler = schedulers.getTemperatureDependentScheduler();
		saveTemperatureDependentScheduler(&schedulersListNode, temperatureDependentScheduler);
	}

	if (schedulers.hasWeeklyScheduler()) {
		const WeeklySchedulerDTO& weeklyScheduler = schedulers.getWeeklyScheduler();
		saveWeeklyScheduler(&schedulersListNode, weeklyScheduler);
	}
}

void XmlWriter::saveEveryDayScheduler(xml_node* parent, const EveryDaySchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("every-day");
}

void XmlWriter::saveHotWeatherScheduler(xml_node* parent, const HotWeatherSchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("hot-weather");

	if (scheduler.hasPeriodInSeconds()) {
		node.append_child("period").text().set(scheduler.getPeriodInSeconds());
	}

	if (scheduler.hasMinTemperature()) {
		node.append_child("temperature").text().set(scheduler.getMinTemperature());
	}
}

void XmlWriter::saveTemperatureDependentScheduler(xml_node* parent, const TemperatureDependentSchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("temperature-dependent");

	if (scheduler.hasRemainingCorrection()) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << scheduler.getRemainingCorrection();
		node.append_child("remaining-correction").text().set(ss.str().c_str());
	}

	if (scheduler.hasMinAdjustment()) {
		node.append_child("min-adjustment").text().set(scheduler.getMinAdjustment());
	}

	if (scheduler.hasMaxAdjustment()) {
		node.append_child("max-adjustment").text().set(scheduler.getMaxAdjustment());
	}
}

void XmlWriter::saveWeeklyScheduler(xml_node* parent, const WeeklySchedulerDTO& scheduler) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("weekly");

	if (scheduler.hasValues()) {
		xml_node daysNode = node.append_child("days");
		const list<bool>& values = scheduler.getValues();
		for (auto value : values) {
			daysNode.append_child("day").text().set(value);
		}
	}
}

void XmlWriter::saveCurrentTemperature(pugi::xml_node* parent, const CurrentTemperatureDTO& currentTemperature) {
	xml_node node = parent->append_child("temperature");

	node.append_child("value").text().set(static_cast<int>(currentTemperature.getValue()));
	node.append_child("datetime").text().set(currentTemperature.getDateTime().c_str());
	node.append_child("unit").text().set(currentTemperature.getUnit().c_str());
}

void XmlWriter::saveTemperatureForecast(pugi::xml_node* parent, const TemperatureForecastDTO& temperatureForecast) {
	xml_node node = parent->append_child("forecast");

	node.append_child("value-min").text().set(static_cast<int>(temperatureForecast.getMinValue()));
	node.append_child("value-max").text().set(static_cast<int>(temperatureForecast.getMaxValue()));
	node.append_child("datetime-from").text().set(temperatureForecast.getDateTimeFrom().c_str());
	node.append_child("datetime-to").text().set(temperatureForecast.getDateTimeTo().c_str());
}

void XmlWriter::saveTemperatureHistory(pugi::xml_node* parent, const TemperatureHistoryDTO& temperatureHistory) {
	xml_node node = parent->append_child("historical");

	node.append_child("value-min").text().set(static_cast<int>(temperatureHistory.getMinValue()));
	node.append_child("value-max").text().set(static_cast<int>(temperatureHistory.getMaxValue()));
	node.append_child("value-avg").text().set(static_cast<int>(temperatureHistory.getAvgValue()));
	node.append_child("datetime-from").text().set(temperatureHistory.getDateTimeFrom().c_str());
	node.append_child("datetime-to").text().set(temperatureHistory.getDateTimeTo().c_str());
}

///////////////////////////////////////////////////////////////////////////////

string XmlWriter::save(const DocumentDTO& document) {
	unique_ptr<xml_document> doc(new xml_document());
	saveDocument(doc.get(), document);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDTO& program) {
	unique_ptr<xml_document> doc(new xml_document());
	saveProgram(doc.get(), program, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDTO& program, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	saveProgram(doc.get(), program, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const list<ProgramDTO>& programs) {
	unique_ptr<xml_document> doc(new xml_document());
	saveProgramList(doc.get(), programs, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const list<ProgramDTO>& programs, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	saveProgramList(doc.get(), programs, false);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const RunTimeDTO& runTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveRunTime(doc.get(), runTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const list<RunTimeDTO>& runTimes) {
	unique_ptr<xml_document> doc(new xml_document());
	saveRunTimeList(doc.get(), runTimes);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDTO& startTime) {
	unique_ptr<xml_document> doc(new xml_document());
	saveStartTime(doc.get(), startTime);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const list<StartTimeDTO>& startTimes) {
	unique_ptr<xml_document> doc(new xml_document());
	saveStartTimeList(doc.get(), startTimes);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const SchedulersDTO& schedulers) {
	unique_ptr<xml_document> doc(new xml_document());
	saveSchedulers(doc.get(), schedulers);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const EveryDaySchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveEveryDayScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const HotWeatherSchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveHotWeatherScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const TemperatureDependentSchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveTemperatureDependentScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const WeeklySchedulerDTO& scheduler) {
	unique_ptr<xml_document> doc(new xml_document());
	saveWeeklyScheduler(doc.get(), scheduler);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const CurrentTemperatureDTO& currentTemperature, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	saveCurrentTemperature(doc.get(), currentTemperature);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureHistoryDTO& temperatureHistory, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureHistory.getUnit().c_str());

	saveTemperatureHistory(&temperatureNode, temperatureHistory);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureForecastDTO& temperatureForecast, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureForecast.getUnit().c_str());

	saveTemperatureForecast(&temperatureNode, temperatureForecast);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureHistoryDTO& temperatureHistory, const TemperatureForecastDTO& temperatureForecast, const std::string& styleSheet) {
	unique_ptr<xml_document> doc(new xml_document());
	prependStyleSheet(doc.get(), styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureForecast.getUnit().c_str());

	saveTemperatureHistory(&temperatureNode, temperatureHistory);
	saveTemperatureForecast(&temperatureNode, temperatureForecast);
	return toString(doc.get(), humanReadable);
}
