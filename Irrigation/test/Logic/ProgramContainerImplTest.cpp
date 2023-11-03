#include "ProgramContainerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "DtoSamples/ProgramDtoSamples.h"
#include <vector>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplConstructorTest, defaultConstructor) {
	ProgramContainerImpl programContainer(std::make_shared<StrictMock<MockProgramFactory>>());

	EXPECT_THAT(programContainer.begin(), Eq(programContainer.end()));
	EXPECT_THAT(programContainer, IsEmpty());
	EXPECT_THAT(programContainer, SizeIs(0));
}

TEST(ProgramContainerImplConstructorTest, initializerConstructor) {
	const std::initializer_list<ProgramContainerImpl::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(20), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(15), std::make_shared<StrictMock<MockProgram>>() },
	};

	ProgramContainerImpl programContainer(initializer);

	ASSERT_THAT(programContainer, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(std::next(programContainer.begin(), i)->first, Eq(std::next(initializer.begin(), i)->first));
		EXPECT_THAT(std::next(programContainer.begin(), i)->second, Eq(std::next(initializer.begin(), i)->second));
	}
}

///////////////////////////////////////////////////////////////////////////////

void ProgramContainerImplTest::SetUp() {
	mockProgramFactory = std::make_shared<StrictMock<MockProgramFactory>>(),
	ON_CALL(*mockProgramFactory, create()).
		WillByDefault(Invoke(mockProgramFactory.get(), &MockProgramFactory::createMockProgram));

	programContainer = std::make_shared<ProgramContainerImpl>(
			mockProgramFactory
		);
}

void ProgramContainerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramContainerImplTest, size) {
	EXPECT_THAT(*programContainer, SizeIs(0));

	programContainer->insert(0, std::make_shared<StrictMock<MockProgram>>());
	EXPECT_THAT(*programContainer, SizeIs(1));

	programContainer->insert(1, std::make_shared<StrictMock<MockProgram>>());
	programContainer->insert(2, std::make_shared<StrictMock<MockProgram>>());
	EXPECT_THAT(*programContainer, SizeIs(3));
}

TEST_F(ProgramContainerImplTest, insert) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ IdType(10), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(20), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(15), std::make_shared<StrictMock<MockProgram>>() },
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*programContainer, ElementsAreArray(items));
}

TEST_F(ProgramContainerImplTest, insert_existing) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ IdType(10), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(20), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(15), std::make_shared<StrictMock<MockProgram>>() },
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	EXPECT_THROW(programContainer->insert(20, std::make_shared<StrictMock<MockProgram>>()), AlreadyExistException);
}

TEST_F(ProgramContainerImplTest, erase) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ 50, std::make_shared<StrictMock<MockProgram>>() },
		{ 40, std::make_shared<StrictMock<MockProgram>>() },
		{ 70, std::make_shared<StrictMock<MockProgram>>() },
		{ 60, std::make_shared<StrictMock<MockProgram>>() }
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*programContainer, SizeIs(items.size()));
	EXPECT_NO_THROW(programContainer->erase(40));
	EXPECT_THAT(*programContainer, SizeIs(items.size() - 1));

	EXPECT_THAT(*programContainer,
			ElementsAre(
				*next(items.begin(), 0),
				*next(items.begin(), 2),
				*next(items.begin(), 3)
			));
}

TEST_F(ProgramContainerImplTest, erase_notExisting) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ 10, std::make_shared<StrictMock<MockProgram>>() },
		{ 20, std::make_shared<StrictMock<MockProgram>>() },
		{ 15, std::make_shared<StrictMock<MockProgram>>() },
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	EXPECT_THAT(*programContainer, SizeIs(items.size()));
	EXPECT_THROW(programContainer->erase(30), NoSuchElementException);
	EXPECT_THAT(*programContainer, SizeIs(items.size()));
}

TEST_F(ProgramContainerImplTest, at) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ 10, std::make_shared<StrictMock<MockProgram>>() },
		{ 20, std::make_shared<StrictMock<MockProgram>>() },
		{ 15, std::make_shared<StrictMock<MockProgram>>() },
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	ASSERT_THAT(*programContainer, SizeIs(items.size()));

	for (const auto& keyValue : items) {
		const ProgramContainerImpl::key_type& requiredKey = keyValue.first;
		const ProgramContainerImpl::mapped_type& requiredValue = keyValue.second;

		EXPECT_THAT(programContainer->at(requiredKey), Eq(requiredValue));
		EXPECT_THAT(Const(programContainer)->at(requiredKey), Eq(requiredValue));
	}
}

TEST_F(ProgramContainerImplTest, at_notExisting) {
	const std::vector<ProgramContainerImpl::value_type> items {
		{ 10, std::make_shared<StrictMock<MockProgram>>() },
		{ 20, std::make_shared<StrictMock<MockProgram>>() },
		{ 15, std::make_shared<StrictMock<MockProgram>>() },
	};

	for (const auto& value : items) {
		programContainer->insert(value.first, value.second);
	}

	EXPECT_THROW(programContainer->at(6), NoSuchElementException);
	EXPECT_THROW(Const(programContainer)->at(6), NoSuchElementException);
}

TEST_F(ProgramContainerImplTest, create) {

	const ProgramDto sample1(DtoSamples::programDtoSample1);
	const ProgramDto sample2(DtoSamples::programDtoSample2);

	auto mockProgram1 = std::make_shared<StrictMock<MockProgram>>();
	auto mockProgram2 = std::make_shared<StrictMock<MockProgram>>();

	ASSERT_THAT(*programContainer, SizeIs(0));
	EXPECT_CALL(*mockProgramFactory, create()).
			Times(2).
			WillOnce(Return(mockProgram1)).
			WillOnce(Return(mockProgram2));

	EXPECT_CALL(*mockProgram1, updateFromProgramDto(sample1)).
			Times(1);

	programContainer->createFromProgramDto(sample1);

	ASSERT_THAT(*programContainer, SizeIs(1));
	EXPECT_THAT(std::next(programContainer->begin(), 0)->second, Eq(mockProgram1));

	EXPECT_CALL(*mockProgram2, updateFromProgramDto(sample2)).
			Times(1);

	programContainer->createFromProgramDto(sample2);

	ASSERT_THAT(*programContainer, SizeIs(2));
	EXPECT_THAT(std::next(programContainer->begin(), 0)->second, Eq(mockProgram1));
	EXPECT_THAT(std::next(programContainer->begin(), 1)->second, Eq(mockProgram2));
}
