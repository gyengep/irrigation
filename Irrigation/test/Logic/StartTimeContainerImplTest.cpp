#include "Logic/StartTimeContainerImpl.h"
#include "Logic/StartTimeImpl.h"
#include "Exceptions/Exceptions.h"
#include "Dto2Object/StartTimeSamples.h"
#include "Mocks/MockStartTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplTest, defaultConstructor) {
	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0));

	EXPECT_THAT(startTimes.begin(), Eq(startTimes.end()));
	EXPECT_THAT(startTimes, IsEmpty());
	EXPECT_THAT(startTimes, SizeIs(0));
}

TEST(StartTimeContainerImplTest, initializerConstructor) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<MockStartTime>() },
		{ IdType(20), std::make_shared<MockStartTime>() },
		{ IdType(15), std::make_shared<MockStartTime>() },
	};

	const StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0), initializer);

	ASSERT_THAT(startTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(startTimes.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(startTimes.begin(), i)->second, Eq(next(initializer.begin(), i)->second));
	}
}

TEST(StartTimeContainerImplTest, size) {
	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0));

	startTimes.insert(IdType(), std::make_shared<MockStartTime>());
	EXPECT_THAT(startTimes, SizeIs(1));

	startTimes.insert(IdType(), std::make_shared<MockStartTime>());
	startTimes.insert(IdType(), std::make_shared<MockStartTime>());
	EXPECT_THAT(startTimes, SizeIs(3));
}

TEST(StartTimeContainerImplTest, insert) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	};

	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0));

	for (const auto& value : initializer) {
		startTimes.insert(value.first, value.second);
	}

	EXPECT_THAT(startTimes, ElementsAreArray(initializer));
}

TEST(StartTimeContainerImplTest, insertExisted) {
	StartTimeContainerImpl startTimes(
			std::make_shared<StrictMock<MockStartTimeFactory>>(0),
			{
				{ 10, std::make_shared<MockStartTime>() },
				{ 20, std::make_shared<MockStartTime>() },
				{ 15, std::make_shared<MockStartTime>() },
			}
		);

	EXPECT_THROW(startTimes.insert(20, std::make_shared<MockStartTime>()), AlreadyExistException);
}

TEST(StartTimeContainerImplTest, erase) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 30, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	};

	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0), initializer);

	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
	EXPECT_NO_THROW(startTimes.erase(30));
	EXPECT_THAT(startTimes, SizeIs(initializer.size() - 1));

	EXPECT_THAT(
		startTimes,
		ElementsAre(
			*next(initializer.begin(), 0),
			*next(initializer.begin(), 1),
			*next(initializer.begin(), 3)
		)
	);
}

TEST(StartTimeContainerImplTest, eraseInvalid) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	};

	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0), initializer);

	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
	EXPECT_THROW(startTimes.erase(30), NoSuchElementException);
	EXPECT_THAT(startTimes, SizeIs(initializer.size()));
}

TEST(StartTimeContainerImplTest, at) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
	};

	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0), initializer);

	ASSERT_THAT(startTimes, SizeIs(3));

	for (size_t i = 0; i < initializer.size(); i++) {
		const IdType& key = std::next(initializer.begin(), 0)->first;
		const StartTimePtr& value = std::next(initializer.begin(), 0)->second;

		EXPECT_THAT(startTimes.at(key), Eq(value));
	}
}

TEST(StartTimeContainerImplTest, atInvalid) {
	StartTimeContainerImpl startTimes(
			std::make_shared<StrictMock<MockStartTimeFactory>>(0),
			{
				{ 10, std::make_shared<MockStartTime>() },
				{ 15, std::make_shared<MockStartTime>() },
				{ 20, std::make_shared<MockStartTime>() },
			}
		);

	EXPECT_THROW(startTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerImplTest, sort) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 0, std::make_shared<StartTimeImpl>(0, 15) },
		{ 1, std::make_shared<StartTimeImpl>(0, 25) },
		{ 2, std::make_shared<StartTimeImpl>(0, 10) },
		{ 3, std::make_shared<StartTimeImpl>(0, 20) },
	};

	StartTimeContainerImpl startTimes(std::make_shared<StrictMock<MockStartTimeFactory>>(0), initializer);
	startTimes.sort();

	EXPECT_THAT(
		startTimes,
		ElementsAre(
			*next(initializer.begin(), 2),
			*next(initializer.begin(), 0),
			*next(initializer.begin(), 3),
			*next(initializer.begin(), 1)
		)
	);
}

TEST(StartTimeContainerImplTest, createFromStartTimeDto) {
	const Dto2ObjectTest::StartTimeSampleList startTimeSampleList;
	auto mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>(startTimeSampleList.size());

	ASSERT_THAT(startTimeSampleList, SizeIs(4));

	StartTimeContainerImpl startTimeContainer(mockStartTimeFactory);

	EXPECT_CALL(*mockStartTimeFactory, create()).Times(startTimeSampleList.size());

	for (size_t i = 0; i < startTimeSampleList.size(); ++i) {
		const StartTimeDTO& startTimeDto = startTimeSampleList[i].getDto();

		EXPECT_CALL(*mockStartTimeFactory->mockStartTimes[i], updateFromStartTimeDto(startTimeDto)).Times(1);

		auto result = startTimeContainer.createFromStartTimeDto(startTimeDto);

		const auto& id = result.first;

		EXPECT_THAT(startTimeContainer, SizeIs(i + 1));
		EXPECT_THAT(startTimeContainer.at(id), Eq(mockStartTimeFactory->mockStartTimes[i]));
	}
}
