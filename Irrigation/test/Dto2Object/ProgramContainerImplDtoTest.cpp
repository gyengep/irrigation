#include "Logic/ProgramContainerImpl.h"
#include "Samples/ProgramContainerSamples.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockSchedulerContainer.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplDtoTest, toProgramContainerDtoList) {
	const Dto2ObjectTestSamples::ProgramContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const ProgramContainerImpl& actual = sample.getContainer();
		const std::list<ProgramDTO>& expected = sample.getDtoList();

		EXPECT_THAT(actual.toProgramDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplDtoTest, createAndInit) {
	const Dto2ObjectTestSamples::ProgramSampleList programSampleList;
	auto mockProgramFactory = std::make_shared<StrictMock<MockProgramFactory>>(programSampleList.size());
	std::vector<StrictMock<MockSchedulerContainer>> mockSchedulerContainers(programSampleList.size());

	ASSERT_THAT(programSampleList, SizeIs(4));

	ProgramContainerImpl programContainer(mockProgramFactory);

	EXPECT_CALL(*mockProgramFactory, create(_)).Times(programSampleList.size());

	for (size_t i = 0; i < programSampleList.size(); ++i) {
		const ProgramDTO& programDto = programSampleList[i].getDto();

		EXPECT_CALL(*mockProgramFactory->mockPrograms[i], updateFromProgramDto(programDto)).
				Times(1);

		EXPECT_CALL(*mockProgramFactory->mockPrograms[i], getSchedulerContainer()).
				Times(1).
				WillOnce(ReturnRef(mockSchedulerContainers[i]));

		EXPECT_CALL(mockSchedulerContainers[i], createPersistedData()).
				Times(1);

		auto result = programContainer.createAndInit(programDto);

		const auto& id = result.first;

		EXPECT_THAT(programContainer, SizeIs(i + 1));
		EXPECT_THAT(programContainer.at(id), Eq(mockProgramFactory->mockPrograms[i]));
	}
}

TEST(ProgramContainerImplDtoTest, deleteAndUnInit) {
	const size_t size = 5;
	const size_t id = 3;

	std::vector<std::shared_ptr<MockProgram>> mockPrograms(size);
	StrictMock<MockSchedulerContainer> mockSchedulerContainer;

	for (size_t i = 0; i < mockPrograms.size(); ++i) {
		mockPrograms[i] = std::make_shared<StrictMock<MockProgram>>();
	}

	EXPECT_CALL(*mockPrograms[3], getSchedulerContainer()).
			Times(1).
			WillOnce(ReturnRef(mockSchedulerContainer));

	EXPECT_CALL(mockSchedulerContainer, deletePersistedData()).
			Times(1);


	ProgramContainerImpl programContainer({
			{ IdType(0), mockPrograms[0] },
			{ IdType(1), mockPrograms[1] },
			{ IdType(2), mockPrograms[2] },
			{ IdType(3), mockPrograms[3] },
			{ IdType(4), mockPrograms[4] },
		});

	programContainer.deleteAndUnInit(id);
	EXPECT_THAT(programContainer, SizeIs(size - 1));
}

///////////////////////////////////////////////////////////////////////////////

void check(std::initializer_list<ProgramContainer::value_type> initializer) {

	for (const auto& sample : Dto2ObjectTestSamples::ProgramContainerSampleList()) {
		const std::list<ProgramDTO>& actualProgramDtoList = sample.getDtoList();
		const size_t size = actualProgramDtoList.size();
		std::vector<StrictMock<MockSchedulerContainer>> mockSchedulerContainers(size);

		auto mockProgramFactory = std::make_shared<StrictMock<MockProgramFactory>>(size);

		EXPECT_CALL(*mockProgramFactory, create(_)).Times(size);

		for (size_t i = 0; i < size; i++) {
			EXPECT_CALL(
					*mockProgramFactory->mockPrograms[i],
					updateFromProgramDto(*std::next(actualProgramDtoList.begin(), i))
				).Times(1);

			EXPECT_CALL(
					*mockProgramFactory->mockPrograms[i],
					getSchedulerContainer()
				).Times(1).
				WillOnce(ReturnRef(mockSchedulerContainers[i]));

			EXPECT_CALL(
					mockSchedulerContainers[i],
					loadPersistedData()
				).Times(1);
		}

		ProgramContainerImpl actualProgramContainer(
			mockProgramFactory,
			initializer
		);

		actualProgramContainer.updateFromProgramDtoList(actualProgramDtoList);

		EXPECT_THAT(actualProgramContainer, SizeIs(size));

		for (size_t i = 0; i < size; i++) {
			EXPECT_THAT(next(actualProgramContainer.begin(), i)->second, Eq(mockProgramFactory->mockPrograms[i]));
		}
	}
}

TEST(ProgramContainerImplFromDtoTest, updateFromProgramContainerDto_zero) {
	std::initializer_list<ProgramContainer::value_type> initializer {
	};

	check(initializer);
}

TEST(ProgramContainerImplFromDtoTest, updateFromProgramContainerDto_one) {
	std::initializer_list<ProgramContainer::value_type> initializer {
		{ IdType(), std::make_shared<StrictMock<MockProgram>>() }
	};

	check(initializer);
}

TEST(ProgramContainerImplFromDtoTest, updateFromProgramContainerDto_more) {
	std::initializer_list<ProgramContainer::value_type> initializer {
		{ IdType(), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(), std::make_shared<StrictMock<MockProgram>>() }
	};

	check(initializer);
}
