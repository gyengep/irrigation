#include "Logic/ProgramContainerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Dto2Object/ProgramSamples.h"
#include "Mocks/MockProgram.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplImplTest, defaultConstructor) {
	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0));

	EXPECT_THAT(programs.begin(), Eq(programs.end()));
	EXPECT_THAT(programs, IsEmpty());
	EXPECT_THAT(programs, SizeIs(0));
}

TEST(ProgramContainerImplImplTest, initializerConstructor) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, std::make_shared<MockProgram>() },
		{ 20, std::make_shared<MockProgram>() },
		{ 15, std::make_shared<MockProgram>() },
	};
	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0), initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(programs.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(programs.begin(), i)->second.get(), Eq(next(initializer.begin(), i)->second.get()));
	}
}

TEST(ProgramContainerImplImplTest, size) {
	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0));
	EXPECT_THAT(programs, SizeIs(0));

	programs.insert(0, std::make_shared<MockProgram>());
	EXPECT_THAT(programs, SizeIs(1));

	programs.insert(1, std::make_shared<MockProgram>());
	programs.insert(2, std::make_shared<MockProgram>());
	EXPECT_THAT(programs, SizeIs(3));
}

TEST(ProgramContainerImplImplTest, insert) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, std::make_shared<MockProgram>() },
		{ 20, std::make_shared<MockProgram>() },
		{ 15, std::make_shared<MockProgram>() },
	};

	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0));

	for (const auto& value : initializer) {
		programs.insert(value.first, value.second);
	}

	EXPECT_THAT(programs, ElementsAreArray(initializer));
}

TEST(ProgramContainerImplImplTest, insertExisting) {
	ProgramContainerImpl programs(
			std::make_shared<StrictMock<MockProgramFactory>>(0),
			{
				{ 100, std::make_shared<MockProgram>() },
				{ 101, std::make_shared<MockProgram>() },
				{ 102, std::make_shared<MockProgram>() },
			}
		);

	EXPECT_THROW(programs.insert(101, std::make_shared<StrictMock<MockProgram>>()), AlreadyExistException);
}

TEST(ProgramContainerImplImplTest, erase) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 50, std::make_shared<MockProgram>() },
		{ 40, std::make_shared<MockProgram>() },
		{ 70, std::make_shared<MockProgram>() },
		{ 60, std::make_shared<MockProgram>() }
	};

	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0), initializer);

	EXPECT_THAT(programs, SizeIs(initializer.size()));
	EXPECT_NO_THROW(programs.erase(40));
	EXPECT_THAT(programs, SizeIs(initializer.size() - 1));

	EXPECT_THAT(programs,
			ElementsAre(
				*next(initializer.begin(), 0),
				*next(initializer.begin(), 2),
				*next(initializer.begin(), 3)
			));
}

TEST(ProgramContainerImplImplTest, eraseInvalid) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, std::make_shared<MockProgram>() },
		{ 20, std::make_shared<MockProgram>() },
		{ 15, std::make_shared<MockProgram>() },
	};

	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0), initializer);

	EXPECT_THAT(programs, SizeIs(initializer.size()));
	EXPECT_THROW(programs.erase(30), NoSuchElementException);
	EXPECT_THAT(programs, SizeIs(initializer.size()));
}

TEST(ProgramContainerImplImplTest, at) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, std::make_shared<MockProgram>() },
		{ 15, std::make_shared<MockProgram>() },
		{ 20, std::make_shared<MockProgram>() },
	};

	ProgramContainerImpl programs(std::make_shared<StrictMock<MockProgramFactory>>(0), initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));

	for (unsigned i = 0; i < initializer.size(); ++i) {
		const ProgramContainerImpl::value_type& value = *next(initializer.begin(), i);
		const ProgramContainerImpl::key_type& requiredKey = value.first;
		const ProgramContainerImpl::mapped_type& requiredValue = value.second;

		EXPECT_THAT(programs.at(requiredKey), Eq(requiredValue));
	}
}

TEST(ProgramContainerImplImplTest, atInvalid) {
	ProgramContainerImpl programs(
			std::make_shared<StrictMock<MockProgramFactory>>(0),
			{
				{ 10, std::make_shared<MockProgram>() },
				{ 15, std::make_shared<MockProgram>() },
				{ 20, std::make_shared<MockProgram>() }
			}
		);

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}


TEST(ProgramContainerImplTest, createFromProgramDto) {
	const Dto2ObjectTest::ProgramSampleList programSampleList;
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
