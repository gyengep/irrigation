#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/ProgramContainer.h"
#include "MockProgram.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramContainerTest, defaultConstructor) {
	ProgramContainer programs;
	EXPECT_THAT(programs.begin(), programs.end());
}

TEST(ProgramContainerTest, initializerConstructor) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{10, shared_ptr<Program>(new Program())},
		{20, shared_ptr<Program>(new Program())},
		{15, shared_ptr<Program>(new Program())},
	};

	ProgramContainer programs(initializer);
	for (size_t i = 0; i < initializer.size(); ++i) {
		EXPECT_THAT(*next(programs.begin(), i), Eq(*next(initializer.begin(), i)));
	}
}

TEST(ProgramContainerTest, size) {
	ProgramContainer programs;
	EXPECT_THAT(programs, SizeIs(0));

	programs.insert(0, shared_ptr<Program>(new Program()));
	EXPECT_THAT(programs, SizeIs(1));
}

TEST(ProgramContainerTest, insert) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{10, shared_ptr<Program>(new Program())},
		{20, shared_ptr<Program>(new Program())},
		{15, shared_ptr<Program>(new Program())},
	};

	ProgramContainer programs;

	for (const auto& value : initializer) {
		programs.insert(value.first, value.second);
	}

	EXPECT_THAT(programs, ElementsAreArray(initializer));
}

TEST(ProgramContainerTest, insertExisting) {
	ProgramContainer programs({
		{100, shared_ptr<Program>(new Program())},
		{101, shared_ptr<Program>(new Program())},
		{102, shared_ptr<Program>(new Program())},
	});

	EXPECT_THROW(programs.insert(101, shared_ptr<Program>(new MockProgram())), AlreadyExistException);
}

TEST(ProgramContainerTest, erase) {
	const initializer_list<ProgramContainer::value_type> initializer {
		{50, shared_ptr<Program>(new Program())},
		{40, shared_ptr<Program>(new Program())},
		{70, shared_ptr<Program>(new Program())},
		{60, shared_ptr<Program>(new Program())}
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
		{10, shared_ptr<Program>(new Program())},
		{20, shared_ptr<Program>(new Program())},
		{15, shared_ptr<Program>(new Program())},
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
		{10, shared_ptr<Program>(new Program())},
		{15, shared_ptr<Program>(new Program())},
		{20, shared_ptr<Program>(new Program())},
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
		{10, shared_ptr<Program>(new Program())},
		{15, shared_ptr<Program>(new Program())},
		{20, shared_ptr<Program>(new Program())},
	});

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}

TEST(ProgramContainerTest, destroyed) {
	ProgramContainer programs;
	programs.insert(0, shared_ptr<Program>(new MockProgram()));
}
