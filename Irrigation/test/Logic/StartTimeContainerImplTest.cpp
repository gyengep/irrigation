#include "StartTimeContainerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "Logic/Impl/StartTimeImpl.h"
#include <vector>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplConstructorTest, defaultConstructor) {
	StartTimeContainerImpl startTimeContainer(std::make_shared<StrictMock<MockStartTimeFactory>>());

	EXPECT_THAT(startTimeContainer.begin(), Eq(startTimeContainer.end()));
	EXPECT_THAT(startTimeContainer, IsEmpty());
	EXPECT_THAT(startTimeContainer, SizeIs(0));
}

TEST(StartTimeContainerImplConstructorTest, initializerConstructor) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<MockStartTime>() },
		{ IdType(20), std::make_shared<MockStartTime>() },
		{ IdType(15), std::make_shared<MockStartTime>() },
	};

	const StartTimeContainerImpl startTimeContainer(initializer);

	ASSERT_THAT(startTimeContainer, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(std::next(startTimeContainer.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(std::next(startTimeContainer.begin(), i)->second, Eq(next(initializer.begin(), i)->second));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplFactoryTest, create) {
	auto mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>();

	StartTimeContainerImplFactory startTimeContainerImplFactory(mockStartTimeFactory);

	EXPECT_THAT(startTimeContainerImplFactory.create(), Not(IsNull()));
	EXPECT_TRUE(startTimeContainerImplFactory.create() != startTimeContainerImplFactory.create());
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplTest::SetUp() {
	mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>();
	ON_CALL(*mockStartTimeFactory, create()).
		WillByDefault(Invoke(mockStartTimeFactory.get(), &MockStartTimeFactory::createMockStartTime));

	startTimeContainer = std::make_shared<StartTimeContainerImpl>(
			mockStartTimeFactory
		);
}

void StartTimeContainerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeContainerImplTest, size) {
	startTimeContainer->insert(IdType(), std::make_shared<MockStartTime>());
	EXPECT_THAT(*startTimeContainer, SizeIs(1));

	startTimeContainer->insert(IdType(), std::make_shared<MockStartTime>());
	startTimeContainer->insert(IdType(), std::make_shared<MockStartTime>());
	EXPECT_THAT(*startTimeContainer, SizeIs(3));
}

TEST_F(StartTimeContainerImplTest, insert) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(15), std::make_shared<StrictMock<MockStartTime>>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*startTimeContainer, ElementsAreArray(initializer));
}

TEST_F(StartTimeContainerImplTest, insert_existing) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(15), std::make_shared<StrictMock<MockStartTime>>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	EXPECT_THROW(startTimeContainer->insert(IdType(20), std::make_shared<StrictMock<MockStartTime>>()), AlreadyExistException);
}

TEST_F(StartTimeContainerImplTest, erase) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 30, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*startTimeContainer, SizeIs(initializer.size()));
	EXPECT_NO_THROW(startTimeContainer->erase(IdType(30)));
	EXPECT_THAT(*startTimeContainer, SizeIs(initializer.size() - 1));

	EXPECT_THAT(
		*startTimeContainer,
		ElementsAre(
			*next(initializer.begin(), 0),
			*next(initializer.begin(), 1),
			*next(initializer.begin(), 3)
		)
	);
}

TEST_F(StartTimeContainerImplTest, erase_notExisting) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(15), std::make_shared<StrictMock<MockStartTime>>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*startTimeContainer, SizeIs(initializer.size()));
	EXPECT_THROW(startTimeContainer->erase(IdType(30)), NoSuchElementException);
	EXPECT_THAT(*startTimeContainer, SizeIs(initializer.size()));
}

TEST_F(StartTimeContainerImplTest, at) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(15), std::make_shared<StrictMock<MockStartTime>>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	ASSERT_THAT(*startTimeContainer, SizeIs(3));

	for (size_t i = 0; i < initializer.size(); i++) {
		const IdType& key = std::next(initializer.begin(), 0)->first;
		const StartTimePtr& value = std::next(initializer.begin(), 0)->second;

		EXPECT_THAT(startTimeContainer->at(key), Eq(value));
		EXPECT_THAT(Const(startTimeContainer)->at(key), Eq(value));
	}
}

TEST_F(StartTimeContainerImplTest, at_notExisting) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(15), std::make_shared<StrictMock<MockStartTime>>() },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	ASSERT_THAT(*startTimeContainer, SizeIs(3));
	EXPECT_THROW(startTimeContainer->at(6), NoSuchElementException);
	EXPECT_THROW(Const(startTimeContainer)->at(6), NoSuchElementException);
}

TEST_F(StartTimeContainerImplTest, sort) {
	const std::vector<StartTimeContainer::value_type> initializer {
		{ IdType(0), std::make_shared<StartTimeImpl>(0, 15) },
		{ IdType(1), std::make_shared<StartTimeImpl>(0, 25) },
		{ IdType(2), std::make_shared<StartTimeImpl>(0, 10) },
		{ IdType(3), std::make_shared<StartTimeImpl>(0, 20) },
	};

	for (const auto& value : initializer) {
		startTimeContainer->insert(value.first, value.second);
	}

	startTimeContainer->sort();

	EXPECT_THAT(
		*startTimeContainer,
		ElementsAre(
			*std::next(initializer.begin(), 2),
			*std::next(initializer.begin(), 0),
			*std::next(initializer.begin(), 3),
			*std::next(initializer.begin(), 1)
		)
	);
}

TEST_F(StartTimeContainerImplTest, create) {
	const StartTimeDto sample1(7, 15);
	const StartTimeDto sample2(8, 30);

	auto mockStartTime1 = std::make_shared<StrictMock<MockStartTime>>();
	auto mockStartTime2 = std::make_shared<StrictMock<MockStartTime>>();

	ASSERT_THAT(*startTimeContainer, SizeIs(0));
	EXPECT_CALL(*mockStartTimeFactory, create()).
			Times(2).
			WillOnce(Return(mockStartTime1)).
			WillOnce(Return(mockStartTime2));

	EXPECT_CALL(*mockStartTime1, updateFromStartTimeDto(sample1)).
			Times(1);

	startTimeContainer->createFromStartTimeDto(sample1);

	ASSERT_THAT(*startTimeContainer, SizeIs(1));
	EXPECT_THAT(std::next(startTimeContainer->begin(), 0)->second, Eq(mockStartTime1));

	EXPECT_CALL(*mockStartTime2, updateFromStartTimeDto(sample2)).
			Times(1);

	startTimeContainer->createFromStartTimeDto(sample2);

	ASSERT_THAT(*startTimeContainer, SizeIs(2));
	EXPECT_THAT(std::next(startTimeContainer->begin(), 0)->second, Eq(mockStartTime1));
	EXPECT_THAT(std::next(startTimeContainer->begin(), 1)->second, Eq(mockStartTime2));
}
