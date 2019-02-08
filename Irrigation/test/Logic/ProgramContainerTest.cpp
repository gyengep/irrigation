#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/ProgramContainer.h"
#include "Dto2Object/ProgramListSamples.h"
#include "Mocks/MockProgram.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerTest, defaultConstructor) {
	ProgramContainer programs;
	EXPECT_THAT(programs.begin(), programs.end());
}

TEST(ProgramContainerTest, initializerConstructor) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, Program::Builder().build() },
		{ 20, Program::Builder().build() },
		{ 15, Program::Builder().build() },
	};

	ProgramContainer programs(initializer);
	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(*next(programs.begin(), i), Eq(*next(initializer.begin(), i)));
	}
}

TEST(ProgramContainerTest, size) {
	ProgramContainer programs;
	EXPECT_THAT(programs, SizeIs(0));

	programs.insert(0, Program::Builder().build());
	EXPECT_THAT(programs, SizeIs(1));
}

TEST(ProgramContainerTest, insert) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, Program::Builder().build() },
		{ 20, Program::Builder().build() },
		{ 15, Program::Builder().build() },
	};

	ProgramContainer programs;

	for (const auto& value : initializer) {
		programs.insert(value.first, value.second);
	}

	EXPECT_THAT(programs, ElementsAreArray(initializer));
}

TEST(ProgramContainerTest, insertExisting) {
	ProgramContainer programs({
		{ 100, Program::Builder().build() },
		{ 101, Program::Builder().build() },
		{ 102, Program::Builder().build() },
	});

	EXPECT_THROW(programs.insert(101, shared_ptr<Program>(new MockProgram())), AlreadyExistException);
}

TEST(ProgramContainerTest, erase) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 50, Program::Builder().build() },
		{ 40, Program::Builder().build() },
		{ 70, Program::Builder().build() },
		{ 60, Program::Builder().build() }
	};

	ProgramContainer programs(initializer);

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

TEST(ProgramContainerTest, eraseInvalid) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, Program::Builder().build() },
		{ 20, Program::Builder().build() },
		{ 15, Program::Builder().build() },
	};

	ProgramContainer programs(initializer);

	EXPECT_THAT(programs, SizeIs(initializer.size()));
	EXPECT_THROW(programs.erase(30), NoSuchElementException);
	EXPECT_THAT(programs, SizeIs(initializer.size()));
}

TEST(ProgramContainerTest, eraseDestroy) {
	ProgramContainer programs;
	programs.insert(0, shared_ptr<Program>(new MockProgram()));
	programs.erase(0);
}

TEST(ProgramContainerTest, at) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, Program::Builder().build() },
		{ 15, Program::Builder().build() },
		{ 20, Program::Builder().build() },
	};

	ProgramContainer programs(initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));
	for (unsigned i = 0; i < initializer.size(); ++i) {
		const ProgramContainer::value_type& value = *next(initializer.begin(), i);
		const ProgramContainer::key_type& requiredKey = value.first;
		const ProgramContainer::mapped_type& requiredValue = value.second;

		EXPECT_THAT(programs.at(requiredKey), Eq(requiredValue));
	}
}

TEST(ProgramContainerTest, atInvalid) {
	ProgramContainer programs({
		{ 10, Program::Builder().build() },
		{ 15, Program::Builder().build() },
		{ 20, Program::Builder().build() },
	});

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}

TEST(ProgramContainerTest, destroyed) {
	ProgramContainer programs;
	programs.insert(0, shared_ptr<Program>(new MockProgram()));
}

///////////////////////////////////////////////////////////////////////////////

void testToProgramDtoList(const ProgramListSample& programListSample) {
	const shared_ptr<ProgramContainer> programContainer = programListSample.getContainer();
	const list<ProgramDTO>& expectedProgramDtoList = programListSample.getDtoList();
	EXPECT_THAT(programContainer->toProgramDtoList(), Eq(expectedProgramDtoList));
}

TEST(ProgramContainerTest, toProgramDtoList_empty) {
	testToProgramDtoList(ProgramListSample1());
}

TEST(ProgramContainerTest, toProgramDtoList_oneItem) {
	testToProgramDtoList(ProgramListSample2());
}

TEST(ProgramContainerTest, toProgramDtoList_moreItem1) {
	testToProgramDtoList(ProgramListSample3());
}

TEST(ProgramContainerTest, toProgramDtoList_moreItem2) {
	testToProgramDtoList(ProgramListSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromProgramDtoList(shared_ptr<ProgramContainer> programContainer, const ProgramListSample& programListSample) {
	EXPECT_THAT(programContainer, Not(Pointee(*programListSample.getContainer())));
	programContainer->updateFromProgramDtoList(programListSample.getDtoList());
	EXPECT_THAT(programContainer, Pointee(*programListSample.getContainer()));
}

TEST(ProgramContainerTest, updateFromProgramDtoList_empty) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample2().getContainer();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample1());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_oneItem) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample3().getContainer();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample2());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_moreItem1) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample1().getContainer();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample3());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_moreItem2) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample2().getContainer();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample4());
}
