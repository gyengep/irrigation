#include "ProgramContainerImplTest.h"
#include "Dto2ObjectSamples/ProgramContainerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplToDtoTest, toProgramContainerDtoList) {
	const Dto2ObjectTestSamples::ProgramContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const ProgramContainerImpl& actual = sample.getContainer();
		const std::list<ProgramDTO>& expected = sample.getDtoList();

		EXPECT_THAT(actual.toProgramDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplFromDtoTest::checkUpdateFromProgramDtoList() {
	const auto sampleList = Dto2ObjectTestSamples::ProgramContainerSampleList();

	unsigned totalCount = 0;
	for (const auto& sample : sampleList) {
		totalCount += sample.getDtoList().size();
	}

	EXPECT_CALL(*mockProgramFactory, create()).
			Times(totalCount).
			WillRepeatedly(Invoke(mockProgramFactory.get(), &MockProgramFactory::createMockProgram));

	for (const auto& sample : sampleList) {
		const size_t expectedSize = sample.getDtoList().size();

		for (const ProgramDTO& programDTO : sample.getDtoList()) {
			mockProgramFactory->mockPrograms.push_back(std::make_shared<StrictMock<MockProgram>>());
			EXPECT_CALL(*mockProgramFactory->mockPrograms.back(), updateFromProgramDto(programDTO)).
					Times(1);
		}

		programContainer->updateFromProgramDtoList(sample.getDtoList());

		EXPECT_THAT(*programContainer, SizeIs(expectedSize));

		for (size_t i = 0; i < expectedSize; i++) {
			EXPECT_THAT(
					std::next(programContainer->begin(), i)->second,
					Eq(mockProgramFactory->mockPrograms[mockProgramFactory->mockPrograms.size() - expectedSize + i])
				);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramContainerImplFromDtoTest, updateFromProgramDtoList_zero) {
	ASSERT_THAT(*programContainer, SizeIs(0));
	checkUpdateFromProgramDtoList();
}

TEST_F(ProgramContainerImplFromDtoTest, updateFromProgramDtoList_one) {
	programContainer->insert(IdType(), std::make_shared<StrictMock<MockProgram>>());

	ASSERT_THAT(*programContainer, SizeIs(1));
	checkUpdateFromProgramDtoList();
}

TEST_F(ProgramContainerImplFromDtoTest, updateFromProgramDtoList_more) {
	programContainer->insert(IdType(), std::make_shared<StrictMock<MockProgram>>());
	programContainer->insert(IdType(), std::make_shared<StrictMock<MockProgram>>());

	ASSERT_THAT(*programContainer, SizeIs(2));
	checkUpdateFromProgramDtoList();
}
