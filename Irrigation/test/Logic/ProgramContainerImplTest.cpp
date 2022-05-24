#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/ProgramContainerImpl.h"
#include "Dto2Object/ProgramListSamples.h"
#include "Mocks/MockProgram.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerImplImplTest, defaultConstructor) {
	ProgramContainerImpl programs;

	EXPECT_THAT(programs.begin(), Eq(programs.end()));
	EXPECT_THAT(programs, IsEmpty());
	EXPECT_THAT(programs, SizeIs(0));
}

TEST(ProgramContainerImplImplTest, initializerConstructor) {
	const initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};
	ProgramContainerImpl programs(initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(programs.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(programs.begin(), i)->second.get(), Eq(next(initializer.begin(), i)->second.get()));
	}
}

TEST(ProgramContainerImplImplTest, equalsOperator) {
	{
		ProgramContainerImpl container1;
		ProgramContainerImpl container2;

		EXPECT_TRUE(container1 == container2);
		EXPECT_TRUE(container2 == container1);

		container1.insert(1000, ProgramSample2().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);

		container2.insert(1000, ProgramSample2().getObjectPtr());
		EXPECT_TRUE(container1 == container2);
		EXPECT_TRUE(container2 == container1);
	}

	{
		ProgramContainerImpl container1;
		ProgramContainerImpl container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container2.insert(1001, ProgramSample2().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		ProgramContainerImpl container1;
		ProgramContainerImpl container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container2.insert(1000, ProgramSample3().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		ProgramContainerImpl container1;
		ProgramContainerImpl container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container1.insert(1001, ProgramSample3().getObjectPtr());
		container2.insert(1001, ProgramSample3().getObjectPtr());
		container2.insert(1000, ProgramSample2().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}
}

TEST(ProgramContainerImplImplTest, size) {
	ProgramContainerImpl programs;
	EXPECT_THAT(programs, SizeIs(0));

	programs.insert(0, ProgramImpl::Builder().build());
	EXPECT_THAT(programs, SizeIs(1));

	programs.insert(1, ProgramImpl::Builder().build());
	programs.insert(2, ProgramImpl::Builder().build());
	EXPECT_THAT(programs, SizeIs(3));
}

TEST(ProgramContainerImplImplTest, insert) {
	const initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};

	ProgramContainerImpl programs;

	for (const auto& value : initializer) {
		programs.insert(value.first, value.second);
	}

	EXPECT_THAT(programs, ElementsAreArray(initializer));
}

TEST(ProgramContainerImplImplTest, insertExisting) {
	ProgramContainerImpl programs({
		{ 100, ProgramImpl::Builder().build() },
		{ 101, ProgramImpl::Builder().build() },
		{ 102, ProgramImpl::Builder().build() },
	});

	EXPECT_THROW(programs.insert(101, ProgramImpl::Builder().build()), AlreadyExistException);
}

TEST(ProgramContainerImplImplTest, erase) {
	const initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 50, ProgramImpl::Builder().build() },
		{ 40, ProgramImpl::Builder().build() },
		{ 70, ProgramImpl::Builder().build() },
		{ 60, ProgramImpl::Builder().build() }
	};

	ProgramContainerImpl programs(initializer);

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
	const initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};

	ProgramContainerImpl programs(initializer);

	EXPECT_THAT(programs, SizeIs(initializer.size()));
	EXPECT_THROW(programs.erase(30), NoSuchElementException);
	EXPECT_THAT(programs, SizeIs(initializer.size()));
}

TEST(ProgramContainerImplImplTest, at) {
	const initializer_list<ProgramContainerImpl::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
	};

	ProgramContainerImpl programs(initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));

	for (unsigned i = 0; i < initializer.size(); ++i) {
		const ProgramContainerImpl::value_type& value = *next(initializer.begin(), i);
		const ProgramContainerImpl::key_type& requiredKey = value.first;
		const ProgramContainerImpl::mapped_type& requiredValue = value.second;

		EXPECT_THAT(programs.at(requiredKey), Eq(requiredValue));
	}
}

TEST(ProgramContainerImplImplTest, atInvalid) {
	ProgramContainerImpl programs({
		{ 10, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
	});

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////

#define PROGRAM_DTO(i)  	ProgramDTO().setName("program_" + std::to_string(i))

void updateFromProgramDtoList(const std::list<ProgramDTO>& programDtoList) {
	std::shared_ptr<MockProgramFactory> mockProgramFactory = std::make_shared<MockProgramFactory>();

	for (size_t i = 0; i < programDtoList.size(); ++i) {
		mockProgramFactory->mockPrograms.push_back(std::make_shared<StrictMock<MockProgram>>());

		EXPECT_CALL(*mockProgramFactory->mockPrograms.back(), updateFromProgramDto(*next(programDtoList.begin(), i)))
			.Times(1);
	}

	ProgramContainerImpl programContainer {
		{ IdType(10), ProgramImpl::Builder().build() },
		{ IdType(20), ProgramImpl::Builder().build() },
		{ IdType(30), ProgramImpl::Builder().build() }
	};

	EXPECT_THAT(programContainer.size(), Eq(3));

	programContainer.updateFromProgramDtoList(mockProgramFactory, programDtoList);
	ASSERT_THAT(programContainer.size(), Eq(programDtoList.size()));

	for (size_t i = 0; i < programDtoList.size(); ++i) {
		const IdType id = next(programDtoList.begin(), i)->getId();

		ASSERT_NO_THROW(programContainer.at(id));
		EXPECT_THAT(programContainer.at(id).get(), Eq(mockProgramFactory->mockPrograms[i].get()));
	}
}


TEST(ProgramContainerImplTest, updateFromProgramDtoList_zeroItem) {
	const std::list<ProgramDTO> programDtoList {
	};

	updateFromProgramDtoList(programDtoList);
}

TEST(ProgramContainerImplTest, updateFromProgramDtoList_oneItem) {
	const std::list<ProgramDTO> programDtoList {
		PROGRAM_DTO(5).setId(5)
	};

	updateFromProgramDtoList(programDtoList);
}

TEST(ProgramContainerImplTest, updateFromProgramDtoList_moreItem) {
	const std::list<ProgramDTO> programDtoList {
		PROGRAM_DTO(10).setId(5),
		PROGRAM_DTO(11).setId(15),
		PROGRAM_DTO(12).setId(20),
		PROGRAM_DTO(13).setId(21),
		PROGRAM_DTO(14).setId(22)
	};

	updateFromProgramDtoList(programDtoList);
}

///////////////////////////////////////////////////////////////////////////////

void toProgramDtoList(const std::initializer_list<ProgramContainer::value_type>& initializer, const std::list<ProgramDTO>& expected) {
	for (size_t i = 0; i < initializer.size(); i++) {
		MockProgram* mockProgram = dynamic_cast<MockProgram*>(next(initializer.begin(), i)->second.get());

		ASSERT_THAT(mockProgram, Not(IsNull()));
		EXPECT_CALL(*mockProgram, toProgramDto())
			.Times(1)
			.WillOnce(Return(PROGRAM_DTO(i)));
	}

	EXPECT_THAT(ProgramContainerImpl(initializer).toProgramDtoList(), ContainerEq(expected));
}

TEST(ProgramContainerImplTest, toProgramDtoList_empty) {
	const std::initializer_list<ProgramContainer::value_type> initializer {
	};

	const std::list<ProgramDTO> expected {
	};

	toProgramDtoList(initializer, expected);
}

TEST(ProgramContainerImplTest, toProgramDtoList_one) {
	const std::initializer_list<ProgramContainer::value_type> initializer {
		{ IdType(10), std::make_shared<StrictMock<MockProgram>>() },
	};

	const std::list<ProgramDTO> expected {
		PROGRAM_DTO(0).setId(10),
	};

	toProgramDtoList(initializer, expected);
}

TEST(ProgramContainerImplTest, toProgramDtoList_more) {
	const std::initializer_list<ProgramContainer::value_type> initializer {
		{ IdType(20), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(21), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(22), std::make_shared<StrictMock<MockProgram>>() },
		{ IdType(23), std::make_shared<StrictMock<MockProgram>>() },
	};

	const std::list<ProgramDTO> expected {
		PROGRAM_DTO(0).setId(20),
		PROGRAM_DTO(1).setId(21),
		PROGRAM_DTO(2).setId(22),
		PROGRAM_DTO(3).setId(23),
	};

	toProgramDtoList(initializer, expected);
}
