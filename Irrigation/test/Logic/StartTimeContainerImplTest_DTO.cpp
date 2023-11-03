#include "StartTimeContainerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const StartTimeDtoList StartTimeContainerImplDtoTest::sampleList_ZeroItem;

const StartTimeDtoList StartTimeContainerImplDtoTest::sampleList_OneItem(
		std::list<StartTimeDto>{
			StartTimeDto(13, 23).setId(48)
		}
	);

const StartTimeDtoList StartTimeContainerImplDtoTest::sampleList_MoreItem(
		std::list<StartTimeDto>{
			StartTimeDto(15, 25).setId(67),
			StartTimeDto(16, 26).setId(71)
		}
	);

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplDtoTest::checkToStartTimeDtoList(const StartTimeDtoList& sampleList) {
	for (const auto& sample : sampleList) {
		auto mockStartTime = std::make_shared<StrictMock<MockStartTime>>();

		EXPECT_CALL(*mockStartTime, toStartTimeDto()).
				Times(1).
				WillOnce(Return(sample));

		startTimeContainer->insert(IdType(sample.getId()), mockStartTime);
	}

	EXPECT_THAT(startTimeContainer->toStartTimeDtoList(), ContainerEq(sampleList));
}

TEST_F(StartTimeContainerImplDtoTest, toStartTimeContainerDtoList_zero) {
	checkToStartTimeDtoList(sampleList_ZeroItem);
}

TEST_F(StartTimeContainerImplDtoTest, toStartTimeContainerDtoList_one) {
	checkToStartTimeDtoList(sampleList_OneItem);
}

TEST_F(StartTimeContainerImplDtoTest, toStartTimeContainerDtoList_more) {
	checkToStartTimeDtoList(sampleList_MoreItem);
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplDtoTest::checkFromStartTimeDtoList(const StartTimeDtoList& sampleList) {

	EXPECT_CALL(*mockStartTimeFactory, create()).
			Times(sampleList.size());

	for (const auto& sample : sampleList) {
		auto mockStartTime = std::make_shared<StrictMock<MockStartTime>>();

		EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(sample)).
				Times(1);

		mockStartTimeFactory->mockStartTimes.push_back(mockStartTime);
	}

	startTimeContainer->updateFromStartTimeDtoList(sampleList);
	ASSERT_THAT(*startTimeContainer, SizeIs(sampleList.size()));

	for (size_t i = 0; i < sampleList.size(); ++i) {
		const IdType id(std::next(sampleList.begin(), i)->getId());
		const auto actualStartTime = startTimeContainer->at(id);
		const auto expectedStartTime = *std::next(mockStartTimeFactory->mockStartTimes.begin(), i);

		EXPECT_THAT(actualStartTime, Eq(expectedStartTime));
	}
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplDtoTest::checkFromStartTimeDtoList_zeroItem() {
	const auto& sampleList = sampleList_ZeroItem;
	ASSERT_THAT(sampleList, SizeIs(0));
	checkFromStartTimeDtoList(sampleList);
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_zero_zero) {
	checkFromStartTimeDtoList_zeroItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_one_zero) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_zeroItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_more_zero) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	startTimeContainer->insert(IdType(101), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_zeroItem();
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplDtoTest::checkFromStartTimeDtoList_oneItem() {
	const auto& sampleList = sampleList_OneItem;
	ASSERT_THAT(sampleList, SizeIs(1));
	checkFromStartTimeDtoList(sampleList);
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_zero_one) {
	checkFromStartTimeDtoList_oneItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_one_one) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_oneItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_more_one) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	startTimeContainer->insert(IdType(101), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_oneItem();
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplDtoTest::checkFromStartTimeDtoList_moreItem() {
	const auto& sampleList = sampleList_MoreItem;
	ASSERT_THAT(sampleList, SizeIs(Gt(1)));
	checkFromStartTimeDtoList(sampleList);
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_zero_more) {
	checkFromStartTimeDtoList_moreItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_one_more) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_moreItem();
}

TEST_F(StartTimeContainerImplDtoTest, updateFromStartTimeDtoList_more_more) {
	startTimeContainer->insert(IdType(100), std::make_shared<StrictMock<MockStartTime>>());
	startTimeContainer->insert(IdType(101), std::make_shared<StrictMock<MockStartTime>>());
	checkFromStartTimeDtoList_moreItem();
}
