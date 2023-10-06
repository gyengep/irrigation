#include "JsonHotWeatherSchedulerRepositoryTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void JsonHotWeatherSchedulerRepositoryTest::SetUp() {
	mockFileReader = std::make_shared<StrictMock<MockFileReader>>();
	mockFileWriterFactory = std::make_shared<StrictMock<MockFileWriterFactory>>();

	repository = std::make_shared<JsonHotWeatherSchedulerRepository>(mockFileReader, mockFileWriterFactory);
}

void JsonHotWeatherSchedulerRepositoryTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(JsonHotWeatherSchedulerRepositoryTest, initial) {
	EXPECT_THAT(repository->getEntities(), IsEmpty());
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, insert) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected {
		{ 1, entity1 },
		{ 5, entity2 },
		{ 2, entity3 }
	};

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity2));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_THAT(repository->getEntities(), ContainerEq(expected));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, insertDuplicated) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected {
		{ 1, entity1 },
		{ 5, entity2 },
		{ 2, entity3 }
	};

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity2));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_THAT(repository->getEntities(), ContainerEq(expected));
	EXPECT_THROW(repository->insert(entity2), AlreadyExistException);
	EXPECT_THAT(repository->getEntities(), ContainerEq(expected));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, erase) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected1 {
		{ 1, entity1 },
		{ 5, entity2 },
		{ 2, entity3 }
	};

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected2 {
		{ 1, entity1 },
		{ 2, entity3 }
	};

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity2));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_THAT(repository->getEntities(), ContainerEq(expected1));
	EXPECT_NO_THROW(repository->erase(entity2));
	EXPECT_THAT(repository->getEntities(), ContainerEq(expected2));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, eraseNotExisting) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_NO_THROW(repository->erase(entity1));
	EXPECT_THROW(repository->erase(entity2), NoSuchElementException);
	EXPECT_NO_THROW(repository->erase(entity3));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, update) {
	const unsigned id = 5;
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2a = std::make_shared<HotWeatherSchedulerEntity>(id, 12340);
	const auto entity2b = std::make_shared<HotWeatherSchedulerEntity>(id, 789);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected1 {
		{ 1, entity1 },
		{ 5, entity2a },
		{ 2, entity3 }
	};

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected2 {
		{ 1, entity1 },
		{ 5, entity2b },
		{ 2, entity3 }
	};

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity2a));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_THAT(repository->getEntities(), ContainerEq(expected1));
	EXPECT_NO_THROW(repository->update(entity2b));
	EXPECT_THAT(repository->getEntities(), ContainerEq(expected2));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, updateNotExisting) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	EXPECT_THROW(repository->update(entity1), NoSuchElementException);
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, findAll) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_THAT(repository->findAll(), IsEmpty());
	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_THAT(repository->findAll(), UnorderedElementsAre(entity1));
	EXPECT_NO_THROW(repository->insert(entity2));
	EXPECT_THAT(repository->findAll(), UnorderedElementsAre(entity1, entity2));
	EXPECT_NO_THROW(repository->insert(entity3));
	EXPECT_THAT(repository->findAll(), UnorderedElementsAre(entity1, entity2, entity3));
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, findById) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(AnyNumber()).
			WillRepeatedly(Return(std::make_shared<NiceMock<MockFileWriter>>()));

	EXPECT_NO_THROW(repository->insert(entity1));
	EXPECT_NO_THROW(repository->insert(entity2));
	EXPECT_NO_THROW(repository->insert(entity3));

	EXPECT_THAT(repository->findById(0), IsNull());
	EXPECT_THAT(repository->findById(1), Pointee(*entity1));
	EXPECT_THAT(repository->findById(2), Pointee(*entity3));
	EXPECT_THAT(repository->findById(3), IsNull());
	EXPECT_THAT(repository->findById(4), IsNull());
	EXPECT_THAT(repository->findById(5), Pointee(*entity2));
	EXPECT_THAT(repository->findById(6), IsNull());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(JsonHotWeatherSchedulerRepositoryTest, save) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::string jsonText1 = "{\n    \"id\": 1,\n    \"lastRun\": 12345\n  }";
	const std::string jsonText5 = "{\n    \"id\": 5,\n    \"lastRun\": 12340\n  }";
	const std::string jsonText2 = "{\n    \"id\": 2,\n    \"lastRun\": 12343\n  }";

	const std::string expected0 = "[\n  " + jsonText1 + "\n]";
	const std::string expected1 = "[\n  " + jsonText1 + ",\n  " + jsonText5 + "\n]";
	const std::string expected2 = "[\n  " + jsonText1 + ",\n  " + jsonText2 + ",\n  " + jsonText5 + "\n]";

	std::shared_ptr<MockFileWriter> mockFileWriter0 = std::make_shared<StrictMock<MockFileWriter>>();
	std::shared_ptr<MockFileWriter> mockFileWriter1 = std::make_shared<StrictMock<MockFileWriter>>();
	std::shared_ptr<MockFileWriter> mockFileWriter2 = std::make_shared<StrictMock<MockFileWriter>>();

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(3).
			WillOnce(Return(mockFileWriter0)).
			WillOnce(Return(mockFileWriter1)).
			WillOnce(Return(mockFileWriter2));

	EXPECT_CALL(*mockFileWriter0, write(expected0)).Times(1);
	EXPECT_CALL(*mockFileWriter1, write(expected1)).Times(1);
	EXPECT_CALL(*mockFileWriter2, write(expected2)).Times(1);

	repository->insert(entity1);
	repository->insert(entity2);
	repository->insert(entity3);
}

TEST_F(JsonHotWeatherSchedulerRepositoryTest, load) {
	const auto entity1 = std::make_shared<HotWeatherSchedulerEntity>(1, 12345);
	const auto entity2 = std::make_shared<HotWeatherSchedulerEntity>(5, 12340);
	const auto entity3 = std::make_shared<HotWeatherSchedulerEntity>(2, 12343);

	const std::map<unsigned, HotWeatherSchedulerEntityPtr> expected {
		{ 1, entity1 },
		{ 5, entity2 },
		{ 2, entity3 }
	};

	const std::string json = R"([{"id":1,"lastRun":12345},{"id":2,"lastRun":12343},{"id":5,"lastRun":12340}])";

	EXPECT_CALL(*mockFileReader, read()).
			Times(1).
			WillOnce(Return(json));

	repository->init();

	EXPECT_THAT(repository->getEntities(), SizeIs(3));
	EXPECT_THAT(repository->findById(1), Pointee(*entity1));
	EXPECT_THAT(repository->findById(5), Pointee(*entity2));
	EXPECT_THAT(repository->findById(2), Pointee(*entity3));
}
