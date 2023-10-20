#include "XmlWriter.h"
#include <pugixml/pugixml.hpp>
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

std::unique_ptr<pugi::xml_document> XmlWriter::createXmlDocument() {
	auto doc = std::unique_ptr<pugi::xml_document>(new pugi::xml_document());
	pugi::xml_node declNode = doc->append_child(node_declaration);
	declNode.append_attribute("version") = "1.0";
	declNode.append_attribute("encoding") = "UTF-8";
	return doc;
}

std::unique_ptr<pugi::xml_document> XmlWriter::createXmlDocument(const std::string& styleSheet) {
	auto doc = createXmlDocument();
	xml_node piNode = doc->append_child(node_pi);
	piNode.set_name("xml-stylesheet");
	piNode.set_value(std::string("type=\"text/xsl\" href=\"" + styleSheet + "\"").c_str());
	return doc;
}

///////////////////////////////////////////////////////////////////////////////

void XmlWriter::saveDocumentDto(xml_node* parent, const DocumentDto& documentDto) {
	xml_node node = parent->append_child("irrigation");

	if (documentDto.hasPrograms()) {
		saveProgramDtoList(&node, documentDto.getPrograms(), true);
	}
}

void XmlWriter::saveProgramDtoList(pugi::xml_node* parent, const ProgramDtoList& programDtoList, bool includeContainers) {
	xml_node programListNode = parent->append_child("programs");

	for (auto& programDto : programDtoList) {
		saveProgramDto(&programListNode, programDto, includeContainers);
	}
}

void XmlWriter::saveProgramDto(xml_node* parent, const ProgramDto& programDto, bool includeContainers) {
	xml_node node = parent->append_child("program");

	if (programDto.hasId()) {
		node.append_attribute("id").set_value(programDto.getId());
	}

	if (programDto.hasName()) {
		node.append_child("name").text().set(programDto.getName().c_str());
	}

	if (programDto.hasEnabled()) {
		node.append_child("enabled").text().set(programDto.getEnabled());
	}

	if (programDto.hasAdjustment()) {
		node.append_child("adjustment").text().set(programDto.getAdjustment());
	}

	if (programDto.hasSchedulerType()) {
		node.append_child("schedulertype").text().set(programDto.getSchedulerType().c_str());
	}

	if (includeContainers) {
		if (programDto.hasSchedulers()) {
			saveSchedulersDto(&node, programDto.getSchedulers());
		}

		if (programDto.hasRunTimes()) {
			saveRunTimeDtoList(&node, programDto.getRunTimes());
		}

		if (programDto.hasStartTimes()) {
			saveStartTimeDtoList(&node, programDto.getStartTimes());
		}
	}
}

void XmlWriter::saveRunTimeDtoList(pugi::xml_node* parent, const RunTimeDtoList& runTimeDtoList) {
	xml_node runTimeListNode = parent->append_child("runtimes");

	for (auto& runTimeDto : runTimeDtoList) {
		saveRunTimeDto(&runTimeListNode, runTimeDto);
	}
}

void XmlWriter::saveRunTimeDto(xml_node* parent, const RunTimeDto& runTimeDto) {
	xml_node node = parent->append_child("runtime");

	if (runTimeDto.hasMinutes()) {
		node.append_child("minute").text().set(runTimeDto.getMinutes());
	}

	if (runTimeDto.hasSeconds()) {
		node.append_child("second").text().set(runTimeDto.getSeconds());
	}

	if (runTimeDto.hasId()) {
		node.append_attribute("id").set_value(runTimeDto.getId());
	}
}

void XmlWriter::saveStartTimeDtoList(pugi::xml_node* parent, const StartTimeDtoList& startTimeDtoList) {
	xml_node startTimeListNode = parent->append_child("starttimes");

	for (auto& startTimeDto : startTimeDtoList) {
		saveStartTimeDto(&startTimeListNode, startTimeDto);
	}
}

void XmlWriter::saveStartTimeDto(xml_node* parent, const StartTimeDto& startTimeDto) {
	xml_node node = parent->append_child("starttime");

	if (startTimeDto.hasHours()) {
		node.append_child("hour").text().set(startTimeDto.getHours());
	}

	if (startTimeDto.hasMinutes()) {
		node.append_child("minute").text().set(startTimeDto.getMinutes());
	}

	if (startTimeDto.hasId()) {
		node.append_attribute("id").set_value(startTimeDto.getId());
	}
}

void XmlWriter::saveSchedulersDto(xml_node* parent, const SchedulersDto& schedulersDto) {
	xml_node schedulersListNode = parent->append_child("schedulers");

	if (schedulersDto.hasEveryDayScheduler()) {
		const EveryDaySchedulerDto& everyDaySchedulerDto = schedulersDto.getEveryDayScheduler();
		saveEveryDaySchedulerDto(&schedulersListNode, everyDaySchedulerDto);
	}

	if (schedulersDto.hasHotWeatherScheduler()) {
		const HotWeatherSchedulerDto& hotWeatherSchedulerDto = schedulersDto.getHotWeatherScheduler();
		saveHotWeatherSchedulerDto(&schedulersListNode, hotWeatherSchedulerDto);
	}

	if (schedulersDto.hasTemperatureDependentScheduler()) {
		const TemperatureDependentSchedulerDto& temperatureDependentSchedulerDto = schedulersDto.getTemperatureDependentScheduler();
		saveTemperatureDependentSchedulerDto(&schedulersListNode, temperatureDependentSchedulerDto);
	}

	if (schedulersDto.hasWeeklyScheduler()) {
		const WeeklySchedulerDto& weeklySchedulerDto = schedulersDto.getWeeklyScheduler();
		saveWeeklySchedulerDto(&schedulersListNode, weeklySchedulerDto);
	}
}

void XmlWriter::saveEveryDaySchedulerDto(xml_node* parent, const EveryDaySchedulerDto& schedulerDto) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("every-day");
}

void XmlWriter::saveHotWeatherSchedulerDto(xml_node* parent, const HotWeatherSchedulerDto& schedulerDto) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("hot-weather");

	if (schedulerDto.hasPeriodInSeconds()) {
		node.append_child("period").text().set(schedulerDto.getPeriodInSeconds());
	}

	if (schedulerDto.hasMinTemperature()) {
		node.append_child("temperature").text().set(schedulerDto.getMinTemperature());
	}
}

void XmlWriter::saveTemperatureDependentSchedulerDto(xml_node* parent, const TemperatureDependentSchedulerDto& schedulerDto) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("temperature-dependent");

	if (schedulerDto.hasRemainingCorrection()) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << schedulerDto.getRemainingCorrection();
		node.append_child("remaining-correction").text().set(ss.str().c_str());
	}

	if (schedulerDto.hasMinAdjustment()) {
		node.append_child("min-adjustment").text().set(schedulerDto.getMinAdjustment());
	}

	if (schedulerDto.hasMaxAdjustment()) {
		node.append_child("max-adjustment").text().set(schedulerDto.getMaxAdjustment());
	}
}

void XmlWriter::saveWeeklySchedulerDto(xml_node* parent, const WeeklySchedulerDto& schedulerDto) {
	xml_node node = parent->append_child("scheduler");
	node.append_attribute("type").set_value("weekly");

	if (schedulerDto.hasValues()) {
		xml_node daysNode = node.append_child("days");
		const list<bool>& values = schedulerDto.getValues();
		for (auto value : values) {
			daysNode.append_child("day").text().set(value);
		}
	}
}

void XmlWriter::saveCurrentTemperatureDto(pugi::xml_node* parent, const CurrentTemperatureDto& currentTemperature) {
	xml_node node = parent->append_child("temperature");

	node.append_child("value").text().set(static_cast<int>(currentTemperature.getValue()));
	node.append_child("datetime").text().set(currentTemperature.getDateTime().c_str());
	node.append_child("unit").text().set(currentTemperature.getUnit().c_str());
}

void XmlWriter::saveTemperatureForecastDto(pugi::xml_node* parent, const TemperatureForecastDto& temperatureForecast) {
	xml_node node = parent->append_child("forecast");

	node.append_child("value-min").text().set(static_cast<int>(temperatureForecast.getMinValue()));
	node.append_child("value-max").text().set(static_cast<int>(temperatureForecast.getMaxValue()));
	node.append_child("datetime-from").text().set(temperatureForecast.getDateTimeFrom().c_str());
	node.append_child("datetime-to").text().set(temperatureForecast.getDateTimeTo().c_str());
}

void XmlWriter::saveTemperatureHistoryDto(pugi::xml_node* parent, const TemperatureHistoryDto& temperatureHistory) {
	xml_node node = parent->append_child("historical");

	node.append_child("value-min").text().set(static_cast<int>(temperatureHistory.getMinValue()));
	node.append_child("value-max").text().set(static_cast<int>(temperatureHistory.getMaxValue()));
	node.append_child("value-avg").text().set(static_cast<int>(temperatureHistory.getAvgValue()));
	node.append_child("datetime-from").text().set(temperatureHistory.getDateTimeFrom().c_str());
	node.append_child("datetime-to").text().set(temperatureHistory.getDateTimeTo().c_str());
}

///////////////////////////////////////////////////////////////////////////////

string XmlWriter::save(const DocumentDto& documentDto) {
	auto doc = createXmlDocument();
	saveDocumentDto(doc.get(), documentDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDto& programDto) {
	auto doc = createXmlDocument();
	saveProgramDto(doc.get(), programDto, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDto& programDto, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);
	saveProgramDto(doc.get(), programDto, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDtoList& programDtoList) {
	auto doc = createXmlDocument();
	saveProgramDtoList(doc.get(), programDtoList, true);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const ProgramDtoList& programDtoList, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);
	saveProgramDtoList(doc.get(), programDtoList, false);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const RunTimeDto& runTimeDto) {
	auto doc = createXmlDocument();
	saveRunTimeDto(doc.get(), runTimeDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const RunTimeDtoList& runTimeDtoList) {
	auto doc = createXmlDocument();
	saveRunTimeDtoList(doc.get(), runTimeDtoList);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDto& startTimeDto) {
	auto doc = createXmlDocument();
	saveStartTimeDto(doc.get(), startTimeDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const StartTimeDtoList& startTimeDtoList) {
	auto doc = createXmlDocument();
	saveStartTimeDtoList(doc.get(), startTimeDtoList);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const SchedulersDto& schedulersDto) {
	auto doc = createXmlDocument();
	saveSchedulersDto(doc.get(), schedulersDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const EveryDaySchedulerDto& schedulerDto) {
	auto doc = createXmlDocument();
	saveEveryDaySchedulerDto(doc.get(), schedulerDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const HotWeatherSchedulerDto& schedulerDto) {
	auto doc = createXmlDocument();
	saveHotWeatherSchedulerDto(doc.get(), schedulerDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const TemperatureDependentSchedulerDto& schedulerDto) {
	auto doc = createXmlDocument();
	saveTemperatureDependentSchedulerDto(doc.get(), schedulerDto);
	return toString(doc.get(), humanReadable);
}

string XmlWriter::save(const WeeklySchedulerDto& schedulerDto) {
	auto doc = createXmlDocument();
	saveWeeklySchedulerDto(doc.get(), schedulerDto);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const CurrentTemperatureDto& currentTemperature, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);
	saveCurrentTemperatureDto(doc.get(), currentTemperature);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureHistoryDto& temperatureHistory, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureHistory.getUnit().c_str());

	saveTemperatureHistoryDto(&temperatureNode, temperatureHistory);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureForecastDto.getUnit().c_str());

	saveTemperatureForecastDto(&temperatureNode, temperatureForecastDto);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const TemperatureHistoryDto& temperatureHistoryDto, const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet) {
	if (temperatureHistoryDto.getUnit() != temperatureForecastDto.getUnit()) {
		throw std::logic_error("temperatureHistory.unit() != temperatureForecast.unit()");
	}

	auto doc = createXmlDocument(styleSheet);

	xml_node temperatureNode = doc->append_child("temperature");
	temperatureNode.append_child("unit").text().set(temperatureForecastDto.getUnit().c_str());

	saveTemperatureHistoryDto(&temperatureNode, temperatureHistoryDto);
	saveTemperatureForecastDto(&temperatureNode, temperatureForecastDto);
	return toString(doc.get(), humanReadable);
}

std::string XmlWriter::save(const LogEntryDtoList& logEntries, const std::string& styleSheet) {
	auto doc = createXmlDocument(styleSheet);

	xml_node logEntriesListNode = doc->append_child("log_entries");
	for (const auto& entry : logEntries) {
		xml_node logEntryNode = logEntriesListNode.append_child("log_entry");

		logEntryNode.append_child("time").text().set(entry.getTime().c_str());
		logEntryNode.append_child("thread").text().set(entry.getThread().c_str());
		logEntryNode.append_child("level").text().set(entry.getLevel().c_str());
		logEntryNode.append_child("text").text().set(entry.getText().c_str());
	}

	return toString(doc.get(), humanReadable);
}
