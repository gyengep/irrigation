#include "Logic/ProgramContainerImpl.h"
#include "Samples/ProgramContainerSamples.h"
#include "Mocks/MockProgram.h"
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

TEST(ProgramContainerImplDtoTest, createFromProgramDto) {
	const Dto2ObjectTestSamples::ProgramSampleList programSampleList;
	auto mockProgramFactory = std::make_shared<StrictMock<MockProgramFactory>>(programSampleList.size());

	ASSERT_THAT(programSampleList, SizeIs(4));

	ProgramContainerImpl programContainer(mockProgramFactory);

	EXPECT_CALL(*mockProgramFactory, create()).Times(programSampleList.size());

	for (size_t i = 0; i < programSampleList.size(); ++i) {
		const ProgramDTO& programDto = programSampleList[i].getDto();

		EXPECT_CALL(*mockProgramFactory->mockPrograms[i], updateFromProgramDto(programDto)).Times(1);

		auto result = programContainer.createFromProgramDto(programDto);

		const auto& id = result.first;

		EXPECT_THAT(programContainer, SizeIs(i + 1));
		EXPECT_THAT(programContainer.at(id), Eq(mockProgramFactory->mockPrograms[i]));
	}
}

///////////////////////////////////////////////////////////////////////////////

void check(std::initializer_list<ProgramContainer::value_type> initializer) {

	for (const auto& sample : Dto2ObjectTestSamples::ProgramContainerSampleList()) {
		const std::list<ProgramDTO>& actualProgramDtoList = sample.getDtoList();
		const size_t size = actualProgramDtoList.size();

		auto mockProgramFactory = std::make_shared<StrictMock<MockProgramFactory>>(size);

		EXPECT_CALL(*mockProgramFactory, create()).Times(size);

		for (size_t i = 0; i < size; i++) {
			EXPECT_CALL(
					*mockProgramFactory->mockPrograms[i],
					updateFromProgramDto(*std::next(actualProgramDtoList.begin(), i))
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
