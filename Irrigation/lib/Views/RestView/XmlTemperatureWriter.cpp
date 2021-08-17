#include "XmlTemperatureWriter.h"
#include "pugixml.hpp"
#include <iomanip>
#include <sstream>

using namespace pugi;


std::string XmlTemperatureWriter::toTemperatureValue(float value) {
	std::ostringstream oss;
	oss << static_cast<int>(value);
	return oss.str();
}

std::string XmlTemperatureWriter::contentType() const {
	return "application/xml";
}

std::string XmlTemperatureWriter::currentToString(const std::string& datetimeStr, float value) const {
	xml_document doc;

	xml_node processingInstructionNode = doc.prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value("type=\"text/xsl\" href=\"/temperature-current.xsl\"");

	xml_node temperatureNode = doc.append_child("temperature");

	temperatureNode.append_child("value").text().set(toTemperatureValue(value).c_str());
	temperatureNode.append_child("datetime").text().set(datetimeStr.c_str());
	temperatureNode.append_child("unit").text().set("celsius");

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

static xml_node createTemperatureNode(xml_document& doc, const std::string& xmlStylSheetName) {
	xml_node temperatureNode = doc.append_child("temperature");
	xml_node processingInstructionNode = doc.prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value(xmlStylSheetName.c_str());

	temperatureNode.append_child("unit").text().set("celsius");

	return temperatureNode;
}

static void addForecast(xml_node& temperatureNode, const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) {
	xml_node forecastNode = temperatureNode.append_child("forecast");

	forecastNode.append_child("value-min").text().set(XmlTemperatureWriter::toTemperatureValue(value.min).c_str());
	forecastNode.append_child("value-max").text().set(XmlTemperatureWriter::toTemperatureValue(value.max).c_str());
	forecastNode.append_child("datetime-from").text().set(datetimeFromStr.c_str());
	forecastNode.append_child("datetime-to").text().set(datetimeToStr.c_str());
}

static void addHistory(xml_node& temperatureNode, const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) {
	xml_node historyNode = temperatureNode.append_child("historical");

	historyNode.append_child("value-min").text().set(XmlTemperatureWriter::toTemperatureValue(value.min).c_str());
	historyNode.append_child("value-max").text().set(XmlTemperatureWriter::toTemperatureValue(value.max).c_str());
	historyNode.append_child("datetime-from").text().set(datetimeFromStr.c_str());
	historyNode.append_child("datetime-to").text().set(datetimeToStr.c_str());
}

std::string XmlTemperatureWriter::tomorrowToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) const {
	xml_document doc;
	xml_node temperatureNode = createTemperatureNode(doc, "type=\"text/xsl\" href=\"/temperature-tomorrow.xsl\"");

	addForecast(temperatureNode, datetimeFromStr, datetimeToStr, value);

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::yesterdayToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) const {
	xml_document doc;
	xml_node temperatureNode = createTemperatureNode(doc, "type=\"text/xsl\" href=\"/temperature-yesterday.xsl\"");

	addHistory(temperatureNode, datetimeFromStr, datetimeToStr, value);

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::todayToString(const std::string& datetimeFromStr, const std::string& datetimeNowStr, const std::string& datetimeToStr, const TemperatureHistory::Values& historyValue, const TemperatureForecast::Values& forecastValue) const {
	xml_document doc;
	xml_node temperatureNode = createTemperatureNode(doc, "type=\"text/xsl\" href=\"/temperature-today.xsl\"");

	addHistory(temperatureNode, datetimeFromStr, datetimeNowStr, historyValue);
	addForecast(temperatureNode, datetimeNowStr, datetimeToStr, forecastValue);

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}
