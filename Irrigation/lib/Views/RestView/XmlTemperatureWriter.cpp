#include "XmlTemperatureWriter.h"
#include "pugixml.hpp"
#include <iomanip>
#include <sstream>

using namespace pugi;


std::string XmlTemperatureWriter::toTemperatureValue(float value) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << value;
	return oss.str();
}

std::string XmlTemperatureWriter::contentType() const {
	return "application/xml";
}

std::string XmlTemperatureWriter::currentToString(float value) const {
	xml_document doc;
	xml_node currentTemperatureNode = doc.append_child("current-temperature");

	currentTemperatureNode.append_child("value").text().set(toTemperatureValue(value).c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::forecastToString(const TemperatureForecast::Values& value, const DateTime& from, const DateTime& to) const {
	xml_document doc;
	xml_node currentTemperatureNode = doc.append_child("temperature-forecast");

	currentTemperatureNode.append_child("min").text().set(toTemperatureValue(value.min).c_str());
	currentTemperatureNode.append_child("max").text().set(toTemperatureValue(value.max).c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");
	currentTemperatureNode.append_child("from").text().set(LocalDateTime(from).toString("%a, %d %b %Y %H:%M:%S %z").c_str());
	currentTemperatureNode.append_child("to").text().set(LocalDateTime(to).toString("%a, %d %b %Y %H:%M:%S %z").c_str());

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

std::string XmlTemperatureWriter::historyToString(const TemperatureHistory::Values& value, const DateTime& from, const DateTime& to) const {
	xml_document doc;
	xml_node currentTemperatureNode = doc.append_child("temperature-history");

	currentTemperatureNode.append_child("min").text().set(toTemperatureValue(value.min).c_str());
	currentTemperatureNode.append_child("max").text().set(toTemperatureValue(value.max).c_str());
	currentTemperatureNode.append_child("unit").text().set("celsius");
	currentTemperatureNode.append_child("from").text().set(LocalDateTime(from).toString("%a, %d %b %Y %H:%M:%S %z").c_str());
	currentTemperatureNode.append_child("to").text().set(LocalDateTime(to).toString("%a, %d %b %Y %H:%M:%S %z").c_str());

	std::ostringstream oss;
	doc.save(oss);
	return oss.str();
}

