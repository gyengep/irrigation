#include "JsonHotWeatherSchedulerRepository.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include <nlohmann/json.hpp>
#include <sstream>


JsonHotWeatherSchedulerRepository::JsonHotWeatherSchedulerRepository(
	const std::shared_ptr<FileReader>& fileReader,
	const std::shared_ptr<FileWriterFactory>& fileWriterFactory
) :
	fileReader(fileReader),
	fileWriterFactory(fileWriterFactory)
{
}

JsonHotWeatherSchedulerRepository::~JsonHotWeatherSchedulerRepository() {
}

void JsonHotWeatherSchedulerRepository::init() {
	std::unique_lock<std::mutex> lock(mtx);

	entities.clear();

	try {
		LOGGER.debug("Loading HotWeatherSchedulerRepository ...");

		nlohmann::json json = nlohmann::json::parse(fileReader->read());
		for (auto it = json.begin(); json.end() != it; ++it) {
			nlohmann::json jsonArrayElement = *it;

			const unsigned id = jsonArrayElement["id"];
			const unsigned lastRun = jsonArrayElement["lastRun"];
			const auto entity = std::make_shared<HotWeatherSchedulerEntity>(id, lastRun);

			LOGGER.debug("  %s", entity->toString().c_str());

			entities.insert(std::make_pair(id, entity));
		}

		LOGGER.debug("HotWeatherSchedulerRepository successfully loaded");
	} catch (const FileNotFoundException&) {
		LOGGER.warning("HotWeatherSchedulerRepository.json file not found");
	}
}

void JsonHotWeatherSchedulerRepository::uninit() {
}

std::list<HotWeatherSchedulerEntityPtr> JsonHotWeatherSchedulerRepository::findAll() const {
	std::unique_lock<std::mutex> lock(mtx);
	std::list<HotWeatherSchedulerEntityPtr> result;

	for (const auto& keyValue : entities) {
		result.push_back(keyValue.second);
	}

	return result;
}

const HotWeatherSchedulerEntityPtr JsonHotWeatherSchedulerRepository::findById(unsigned id) const {
	std::unique_lock<std::mutex> lock(mtx);

	const auto& it = entities.find(id);
	if (entities.end() == it) {
		return nullptr;
	}

	return it->second;
}

void JsonHotWeatherSchedulerRepository::save_unsafe() const {
	nlohmann::json json;
	for (const auto& keyValue : entities) {
		const auto& value = keyValue.second;

		nlohmann::json jsonArrayElement;
		jsonArrayElement["id"] = value->getId();
		jsonArrayElement["lastRun"] = value->getLastRun();

		json.push_back(jsonArrayElement);
	}

	fileWriterFactory->create(FileWriter::Type::TRUNCATE)->write(json.dump(2));
}

void JsonHotWeatherSchedulerRepository::insert(const HotWeatherSchedulerEntityPtr& entity) {
	std::unique_lock<std::mutex> lock(mtx);

	if (entities.end() != entities.find(entity->getId())) {
		throw AlreadyExistException("HotWeatherSchedulerEntity[" + std::to_string(entity->getId()) + "] already exists");
	}

	entities.insert(std::make_pair(entity->getId(), entity));
	save_unsafe();
}

void JsonHotWeatherSchedulerRepository::erase(const HotWeatherSchedulerEntityPtr& entity) {
	std::unique_lock<std::mutex> lock(mtx);

	const auto it = entities.find(entity->getId());
	if (it == entities.end()) {
		throw NoSuchElementException("HotWeatherSchedulerEntity[" + std::to_string(entity->getId()) + "] does not exist");
	}

	entities.erase(it);
	save_unsafe();
}

void JsonHotWeatherSchedulerRepository::update(const HotWeatherSchedulerEntityPtr& entity) {
	std::unique_lock<std::mutex> lock(mtx);

	const auto it = entities.find(entity->getId());
	if (it == entities.end()) {
		throw NoSuchElementException("HotWeatherSchedulerEntity[" + std::to_string(entity->getId()) + "] does not exist");
	}

	it->second = entity;
	save_unsafe();
}

std::string JsonHotWeatherSchedulerRepository::toString() const {
	std::ostringstream oss;

	oss << "[";
	for (auto it = entities.begin(); entities.end() != it; ++it) {
		if (it != entities.begin()) {
			oss << ", ";
		}
		oss << "{" << it->second->toString() << "}";
	}
	oss << "]";

	return oss.str();
}
