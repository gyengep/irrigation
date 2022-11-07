#include <gmock/gmock.h>
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Logic/StartTimeImpl.h"
#include "Mocks/MockStartTime.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplTest, defaultConstructor) {
	StartTimeContainerImpl startTimes;

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
	const StartTimeContainerImpl startTimes(initializer);

	ASSERT_THAT(startTimes, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(startTimes.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(startTimes.begin(), i)->second, Eq(next(initializer.begin(), i)->second));
	}
}

TEST(StartTimeContainerImplTest, size) {
	StartTimeContainerImpl startTimes;

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

	StartTimeContainerImpl startTimes;

	for (const auto& value : initializer) {
		startTimes.insert(value.first, value.second);
	}

	EXPECT_THAT(startTimes, ElementsAreArray(initializer));
}

TEST(StartTimeContainerImplTest, insertExisted) {
	StartTimeContainerImpl startTimes {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	} ;

	EXPECT_THROW(startTimes.insert(20, std::make_shared<MockStartTime>()), AlreadyExistException);
}

TEST(StartTimeContainerImplTest, erase) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 10, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
		{ 30, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
	};

	StartTimeContainerImpl startTimes(initializer);

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

	StartTimeContainerImpl startTimes(initializer);

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

	StartTimeContainerImpl startTimes(initializer);

	ASSERT_THAT(startTimes, SizeIs(3));

	for (size_t i = 0; i < initializer.size(); i++) {
		const IdType& key = std::next(initializer.begin(), 0)->first;
		const StartTimePtr& value = std::next(initializer.begin(), 0)->second;

		EXPECT_THAT(startTimes.at(key), Eq(value));
	}
}

TEST(StartTimeContainerImplTest, atInvalid) {
	StartTimeContainerImpl startTimes {
		{ 10, std::make_shared<MockStartTime>() },
		{ 15, std::make_shared<MockStartTime>() },
		{ 20, std::make_shared<MockStartTime>() },
	} ;

	EXPECT_THROW(startTimes.at(6), NoSuchElementException);
}

TEST(StartTimeContainerImplTest, sort) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ 0, std::make_shared<StartTimeImpl>(0, 15) },
		{ 1, std::make_shared<StartTimeImpl>(0, 25) },
		{ 2, std::make_shared<StartTimeImpl>(0, 10) },
		{ 3, std::make_shared<StartTimeImpl>(0, 20) },
	};

	StartTimeContainerImpl startTimes(initializer);
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

///////////////////////////////////////////////////////////////////////////////

#define START_TIME_DTO(i)  	StartTimeDTO().setHours(10 + i).setMinutes(20 + i)


void updateFromStartTimeDtoList(const std::list<StartTimeDTO>& startTimeDtoList) {
	std::shared_ptr<MockStartTimeFactory> mockStartTimeFactory = std::make_shared<MockStartTimeFactory>(startTimeDtoList.size());

	for (size_t i = 0; i < startTimeDtoList.size(); ++i) {
		EXPECT_CALL(*mockStartTimeFactory->mockStartTimes[i], updateFromStartTimeDto(*next(startTimeDtoList.begin(), i)))
			.Times(1);
	}

	StartTimeContainerImpl startTimeContainer {
		{ IdType(10), std::make_shared<MockStartTime>() },
		{ IdType(20), std::make_shared<MockStartTime>() },
		{ IdType(30), std::make_shared<MockStartTime>() }
	};

	EXPECT_THAT(startTimeContainer, SizeIs(3));

	startTimeContainer.updateFromStartTimeDtoList(mockStartTimeFactory, startTimeDtoList);
	ASSERT_THAT(startTimeContainer.size(), Eq(startTimeDtoList.size()));

	for (size_t i = 0; i < startTimeDtoList.size(); ++i) {
		const IdType id = next(startTimeDtoList.begin(), i)->getId();

		ASSERT_NO_THROW(startTimeContainer.at(id));
		EXPECT_THAT(startTimeContainer.at(id), Eq(mockStartTimeFactory->mockStartTimes[i]));
	}
}


TEST(StartTimeContainerImplTest, updateDtoList_zeroItem) {
	const std::list<StartTimeDTO> startTimeDtoList {
	};

	updateFromStartTimeDtoList(startTimeDtoList);
}

TEST(StartTimeContainerImplTest, updateFromDtoList_oneItem) {
	const std::list<StartTimeDTO> startTimeDtoList {
		START_TIME_DTO(5).setId(5)
	};

	updateFromStartTimeDtoList(startTimeDtoList);
}

TEST(StartTimeContainerImplTest, updateFromDtoList_moreItem) {
	const std::list<StartTimeDTO> startTimeDtoList {
		START_TIME_DTO(10).setId(5),
		START_TIME_DTO(11).setId(15),
		START_TIME_DTO(12).setId(20),
		START_TIME_DTO(13).setId(21),
		START_TIME_DTO(14).setId(22)
	};

	updateFromStartTimeDtoList(startTimeDtoList);
}

///////////////////////////////////////////////////////////////////////////////

void toStartTimeDtoList(const std::initializer_list<StartTimeContainer::value_type>& initializer, const std::list<StartTimeDTO>& expected) {
	for (size_t i = 0; i < initializer.size(); i++) {
		MockStartTime* mockStartTime = dynamic_cast<MockStartTime*>(next(initializer.begin(), i)->second.get());

		ASSERT_THAT(mockStartTime, Not(IsNull()));
		EXPECT_CALL(*mockStartTime, toStartTimeDto())
			.Times(1)
			.WillOnce(Return(START_TIME_DTO(i)));
	}

	EXPECT_THAT(StartTimeContainerImpl(initializer).toStartTimeDtoList(), ContainerEq(expected));
}

TEST(StartTimeContainerImplTest, toDtoList_empty) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
	};

	const std::list<StartTimeDTO> expected {
	};

	toStartTimeDtoList(initializer, expected);
}

TEST(StartTimeContainerImplTest, toDtoList_one) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockStartTime>>() },
	};

	const std::list<StartTimeDTO> expected {
		START_TIME_DTO(0).setId(10),
	};

	toStartTimeDtoList(initializer, expected);
}

TEST(StartTimeContainerImplTest, toDtoList_more) {
	const std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(20), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(21), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(22), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(23), std::make_shared<StrictMock<MockStartTime>>() },
	};

	const std::list<StartTimeDTO> expected {
		START_TIME_DTO(0).setId(20),
		START_TIME_DTO(1).setId(21),
		START_TIME_DTO(2).setId(22),
		START_TIME_DTO(3).setId(23),
	};

	toStartTimeDtoList(initializer, expected);
}
