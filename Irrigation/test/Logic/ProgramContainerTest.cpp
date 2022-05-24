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

	EXPECT_THAT(programs.begin(), Eq(programs.end()));
	EXPECT_THAT(programs, IsEmpty());
	EXPECT_THAT(programs, SizeIs(0));
}

TEST(ProgramContainerTest, initializerConstructor) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};
	ProgramContainer programs(initializer);

	ASSERT_THAT(programs, SizeIs(initializer.size()));

	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(next(programs.begin(), i)->first, Eq(next(initializer.begin(), i)->first));
		EXPECT_THAT(next(programs.begin(), i)->second.get(), Eq(next(initializer.begin(), i)->second.get()));
	}
}

TEST(ProgramContainerTest, equalsOperator) {
	{
		ProgramContainer container1;
		ProgramContainer container2;

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
		ProgramContainer container1;
		ProgramContainer container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container2.insert(1001, ProgramSample2().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		ProgramContainer container1;
		ProgramContainer container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container2.insert(1000, ProgramSample3().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}

	{
		ProgramContainer container1;
		ProgramContainer container2;

		container1.insert(1000, ProgramSample2().getObjectPtr());
		container1.insert(1001, ProgramSample3().getObjectPtr());
		container2.insert(1001, ProgramSample3().getObjectPtr());
		container2.insert(1000, ProgramSample2().getObjectPtr());
		EXPECT_FALSE(container1 == container2);
		EXPECT_FALSE(container2 == container1);
	}
}

TEST(ProgramContainerTest, size) {
	ProgramContainer programs;
	EXPECT_THAT(programs, SizeIs(0));

	programs.insert(0, ProgramImpl::Builder().build());
	EXPECT_THAT(programs, SizeIs(1));
}

TEST(ProgramContainerTest, insert) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};

	ProgramContainer programs;

	for (const auto& value : initializer) {
		programs.insert(value.first, value.second);
	}

	EXPECT_THAT(programs, ElementsAreArray(initializer));
}

TEST(ProgramContainerTest, insertExisting) {
	ProgramContainer programs({
		{ 100, ProgramImpl::Builder().build() },
		{ 101, ProgramImpl::Builder().build() },
		{ 102, ProgramImpl::Builder().build() },
	});

	EXPECT_THROW(programs.insert(101, ProgramImpl::Builder().build()), AlreadyExistException);
}

TEST(ProgramContainerTest, erase) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 50, ProgramImpl::Builder().build() },
		{ 40, ProgramImpl::Builder().build() },
		{ 70, ProgramImpl::Builder().build() },
		{ 60, ProgramImpl::Builder().build() }
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
		{ 10, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
	};

	ProgramContainer programs(initializer);

	EXPECT_THAT(programs, SizeIs(initializer.size()));
	EXPECT_THROW(programs.erase(30), NoSuchElementException);
	EXPECT_THAT(programs, SizeIs(initializer.size()));
}

TEST(ProgramContainerTest, at) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{ 10, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
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
		{ 10, ProgramImpl::Builder().build() },
		{ 15, ProgramImpl::Builder().build() },
		{ 20, ProgramImpl::Builder().build() },
	});

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}

///////////////////////////////////////////////////////////////////////////////
/*
void testToProgramDtoList(const ProgramListSample& programListSample) {
	const shared_ptr<ProgramContainer> programContainer = programListSample.getContainerPtr();
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

void testUpdateFromProgramDtoList(const shared_ptr<ProgramContainer>& programContainer, const ProgramListSample& programListSample) {
	EXPECT_THAT(programContainer, Pointee(Not(Eq(std::ref(*programListSample.getContainerPtr())))));
	programContainer->updateFromProgramDtoList(programListSample.getDtoList());
	EXPECT_THAT(programContainer, Pointee(Eq(std::ref(*programListSample.getContainerPtr()))));
}

TEST(ProgramContainerTest, updateFromProgramDtoList_empty) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample2().getContainerPtr();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample1());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_oneItem) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample3().getContainerPtr();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample2());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_moreItem1) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample1().getContainerPtr();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample3());
}

TEST(ProgramContainerTest, updateFromProgramDtoList_moreItem2) {
	shared_ptr<ProgramContainer> programContainer = ProgramListSample2().getContainerPtr();
	testUpdateFromProgramDtoList(programContainer, ProgramListSample4());
}
*/
