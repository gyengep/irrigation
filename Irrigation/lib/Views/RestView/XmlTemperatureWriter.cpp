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
	processingInstructionNode.set_value("type=\"text/xsl\" href=\"/resources/temperature.xsl\"");

	xml_node currentTemperatureNode = doc.append_child("temperature");

	currentTemperatureNode.append_child("value").text().set(toTemperatureValue(value).c_str());
	currentTemperatureNode.append_child("datetime").text().set(datetimeStr.c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::forecastToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureForecast::Values& value) const {
	xml_document doc;
	xml_node currentTemperatureNode = doc.append_child("temperature-forecast");

	xml_node processingInstructionNode = doc.prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value("type=\"text/xsl\" href=\"/resources/temperature-forecast.xsl\"");

	currentTemperatureNode.append_child("value-min").text().set(toTemperatureValue(value.min).c_str());
	currentTemperatureNode.append_child("value-max").text().set(toTemperatureValue(value.max).c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");
	currentTemperatureNode.append_child("datetime-from").text().set(datetimeFromStr.c_str());
	currentTemperatureNode.append_child("datetime-to").text().set(datetimeToStr.c_str());

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::historyToString(const std::string& datetimeFromStr, const std::string& datetimeToStr, const TemperatureHistory::Values& value) const {
	xml_document doc;
	xml_node currentTemperatureNode = doc.append_child("temperature-history");

	xml_node processingInstructionNode = doc.prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value("type=\"text/xsl\" href=\"/resources/temperature-history.xsl\"");

	currentTemperatureNode.append_child("value-min").text().set(toTemperatureValue(value.min).c_str());
	currentTemperatureNode.append_child("value-max").text().set(toTemperatureValue(value.max).c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");
	currentTemperatureNode.append_child("datetime-from").text().set(datetimeFromStr.c_str());
	currentTemperatureNode.append_child("datetime-to").text().set(datetimeToStr.c_str());

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

