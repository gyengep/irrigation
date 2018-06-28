#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"


using namespace std;


class MockProgram : public Program {
public:
	MockProgram() {
		EXPECT_CALL(*this, destructorIsCalled()).Times(1);
	}

	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockProgram() { destructorIsCalled(); }
};

typedef vector<pair<const IdType, Program*>> IdTypeProgramPtrVector;

static void insertToPrograms(ProgramContainer& programs, const IdTypeProgramPtrVector& values) {
	for (unsigned i = 0; i < values.size(); ++i) {
		programs.insert(values[i].first, values[i].second);
	}
}

static void expectPrograms(const ProgramContainer& programs, const IdTypeProgramPtrVector& required) {
	ASSERT_EQ(required.size(), programs.size());

	unsigned i = 0;
	for (auto it = programs.begin(); it != programs.end(); ++it, ++i) {
		EXPECT_EQ(required[i].first, it->first);
		EXPECT_EQ(required[i].second, it->second.get());
	}
}


TEST(ProgramContainerTest, size) {
	ProgramContainer programs;
	EXPECT_EQ(0, programs.size());

	programs.insert(0, new Program());
	EXPECT_EQ(1, programs.size());
}

TEST(ProgramContainerTest, insert) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{20, new Program()},
		{15, new Program()},
	};

	ProgramContainer programs;

	for (auto& requiredPair : required) {
		const ProgramContainer::value_type& insertedPair = programs.insert(requiredPair.first, requiredPair.second);

		EXPECT_EQ(requiredPair.first, insertedPair.first);
		EXPECT_EQ(requiredPair.second, insertedPair.second.get());
	}

	expectPrograms(programs, required);
}

TEST(ProgramContainerTest, insertExisting) {
	const IdTypeProgramPtrVector required {
		{100, new Program()},
		{101, new Program()},
		{102, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_THROW(programs.insert(101, new MockProgram()), AlreadyExistException);
}

TEST(ProgramContainerTest, erase) {
	const IdTypeProgramPtrVector programsToAdd {
		{50, new Program()},
		{40, new Program()},
		{70, new Program()},
		{60, new Program()}
	};

	ProgramContainer programs;
	insertToPrograms(programs, programsToAdd);
	ASSERT_EQ(programsToAdd.size(), programs.size());

	programs.erase(40);

	const IdTypeProgramPtrVector required {
		{50, programsToAdd[0].second},
		{70, programsToAdd[2].second},
		{60, programsToAdd[3].second},
	};

	expectPrograms(programs, required);
}

TEST(ProgramContainerTest, eraseInvalid) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{20, new Program()},
		{15, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_EQ(required.size(), programs.size());
	EXPECT_THROW(programs.erase(30), NoSuchElementException);
	EXPECT_EQ(required.size(), programs.size());
}

TEST(ProgramContainerTest, eraseDestructed) {
	ProgramContainer programs;
	programs.insert(0, new MockProgram());
	programs.erase(0);
}

TEST(ProgramContainerTest, at) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	ASSERT_EQ(required.size(), programs.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		const IdType& requiredId = required[i].first;
		const Program* requiredPtr = required[i].second;

		Program* actualProgram = programs.at(requiredId);
		EXPECT_EQ(requiredPtr, actualProgram);
	}
}

TEST(ProgramContainerTest, atInvalid) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_THROW(programs.at(6), NoSuchElementException);
}

TEST(ProgramContainerTest, atConst) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	const ProgramContainer& constPrograms = programs;

	ASSERT_EQ(required.size(), programs.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		const IdType& requiredId = required[i].first;
		const Program* requiredPtr = required[i].second;

		EXPECT_EQ(requiredPtr, constPrograms.at(requiredId));
	}
}

TEST(ProgramContainerTest, atConstInvalid) {
	const IdTypeProgramPtrVector required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	const ProgramContainer& constPrograms = programs;
	EXPECT_THROW(constPrograms.at(6), NoSuchElementException);
}

TEST(ProgramContainerTest, destructed) {
	ProgramContainer programs;
	programs.insert(0, new MockProgram());
}
