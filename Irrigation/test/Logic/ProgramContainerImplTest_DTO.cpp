#include "ProgramContainerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "DtoSamples/ProgramDtoSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const ProgramDtoList ProgramContainerImplDtoTest::sampleList_ZeroItem;

const ProgramDtoList ProgramContainerImplDtoTest::sampleList_OneItem(
		std::list<ProgramDto>{
			ProgramDto(DtoSamples::programDtoSample1).setId(48)
		}
	);

const ProgramDtoList ProgramContainerImplDtoTest::sampleList_MoreItem(
		std::list<ProgramDto>{
			ProgramDto(DtoSamples::programDtoSample2).setId(69),
			ProgramDto(DtoSamples::programDtoSample3).setId(75)
		}
	);

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplDtoTest::checkToProgramDtoList(const ProgramDtoList& sampleList) {
	for (const auto& sample : sampleList) {
		auto mockProgram = std::make_shared<StrictMock<MockProgram>>();

		EXPECT_CALL(*mockProgram, toProgramDto()).
				Times(1).
				WillOnce(Return(sample));

		programContainer->insert(IdType(sample.getId()), mockProgram);
	}

	EXPECT_THAT(programContainer->toProgramDtoList(), ContainerEq(sampleList));
}

TEST_F(ProgramContainerImplDtoTest, toProgramContainerDtoList_zero) {
	checkToProgramDtoList(sampleList_ZeroItem);
}

TEST_F(ProgramContainerImplDtoTest, toProgramContainerDtoList_one) {
	checkToProgramDtoList(sampleList_OneItem);
}

TEST_F(ProgramContainerImplDtoTest, toProgramContainerDtoList_more) {
	checkToProgramDtoList(sampleList_MoreItem);
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplDtoTest::checkFromProgramDtoList(const ProgramDtoList& sampleList) {

	EXPECT_CALL(*mockProgramFactory, create()).
			Times(sampleList.size());

	for (const auto& sample : sampleList) {
		auto mockProgram = std::make_shared<StrictMock<MockProgram>>();

		EXPECT_CALL(*mockProgram, updateFromProgramDto(sample)).
				Times(1);

		mockProgramFactory->mockPrograms.push_back(mockProgram);
	}

	programContainer->updateFromProgramDtoList(sampleList);
	ASSERT_THAT(*programContainer, SizeIs(sampleList.size()));

	for (size_t i = 0; i < sampleList.size(); ++i) {
		const IdType id(std::next(sampleList.begin(), i)->getId());
		const auto actualProgram = programContainer->at(id);
		const auto expectedProgram = *std::next(mockProgramFactory->mockPrograms.begin(), i);

		EXPECT_THAT(actualProgram, Eq(expectedProgram));
	}
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplDtoTest::checkFromProgramDtoList_zeroItem() {
	const auto& sampleList = sampleList_ZeroItem;
	ASSERT_THAT(sampleList, SizeIs(0));
	checkFromProgramDtoList(sampleList);
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_zero_zero) {
	checkFromProgramDtoList_zeroItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_one_zero) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_zeroItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_more_zero) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	programContainer->insert(IdType(101), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_zeroItem();
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplDtoTest::checkFromProgramDtoList_oneItem() {
	const auto& sampleList = sampleList_OneItem;
	ASSERT_THAT(sampleList, SizeIs(1));
	checkFromProgramDtoList(sampleList);
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_zero_one) {
	checkFromProgramDtoList_oneItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_one_one) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_oneItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_more_one) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	programContainer->insert(IdType(101), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_oneItem();
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplDtoTest::checkFromProgramDtoList_moreItem() {
	const auto& sampleList = sampleList_MoreItem;
	ASSERT_THAT(sampleList, SizeIs(Gt(1)));
	checkFromProgramDtoList(sampleList);
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_zero_more) {
	checkFromProgramDtoList_moreItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_one_more) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_moreItem();
}

TEST_F(ProgramContainerImplDtoTest, updateFromProgramDtoList_more_more) {
	programContainer->insert(IdType(100), std::make_shared<StrictMock<MockProgram>>());
	programContainer->insert(IdType(101), std::make_shared<StrictMock<MockProgram>>());
	checkFromProgramDtoList_moreItem();
}
