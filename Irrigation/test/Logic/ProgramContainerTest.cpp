#include <gmock/gmock.h>
#include "Logic/Exceptions.h"
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

static void insertToPrograms(ProgramContainer& programs, const vector<ProgramContainer::value_type>& values) {
	for (unsigned i = 0; i < values.size(); ++i) {
		programs.insert(values[i].first, values[i].second);
	}
}

static void expectPrograms(const ProgramContainer& programs, const vector<ProgramContainer::value_type>& required) {
	ASSERT_EQ(required.size(), programs.size());

	unsigned i = 0;
	for (auto it = programs.begin(); it != programs.end(); ++it, ++i) {
		EXPECT_EQ(required[i], *it);
	}
}


TEST(ProgramContainerTest, size) {
	ProgramContainer programs;
	EXPECT_EQ(0, programs.size());

	programs.insert(0, new Program());
	EXPECT_EQ(1, programs.size());
}

TEST(ProgramContainerTest, insert) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{20, new Program()},
		{15, new Program()},
	};

	ProgramContainer programs;

	for (auto& pair : required) {
		EXPECT_EQ(pair, programs.insert(pair.first, pair.second));
	}

	expectPrograms(programs, required);
}

TEST(ProgramContainerTest, insertExisting) {
	const vector<ProgramContainer::value_type> required {
		{100, new Program()},
		{101, new Program()},
		{102, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_THROW(programs.insert(101, new MockProgram()), ProgramIdExist);
}

TEST(ProgramContainerTest, erase) {
	const vector<ProgramContainer::value_type> programsToAdd {
		{50, new Program()},
		{40, new Program()},
		{70, new Program()},
		{60, new Program()}
	};

	ProgramContainer programs;
	insertToPrograms(programs, programsToAdd);
	ASSERT_EQ(programsToAdd.size(), programs.size());

	programs.erase(40);

	const vector<ProgramContainer::value_type> required {
		{50, programsToAdd[0].second},
		{70, programsToAdd[2].second},
		{60, programsToAdd[3].second},
	};

	expectPrograms(programs, required);
}

TEST(ProgramContainerTest, eraseInvalid) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{20, new Program()},
		{15, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_EQ(required.size(), programs.size());
	EXPECT_THROW(programs.erase(30), InvalidProgramIdException);
	EXPECT_EQ(required.size(), programs.size());
}

TEST(ProgramContainerTest, eraseDestructed) {
	ProgramContainer programs;
	programs.insert(0, new MockProgram());
	programs.erase(0);
}

TEST(ProgramContainerTest, at) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	ASSERT_EQ(required.size(), programs.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		EXPECT_EQ(required[i].second, programs.at(required[i].first));
	}
}

TEST(ProgramContainerTest, atConst) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	const ProgramContainer& constPrograms = programs;

	insertToPrograms(programs, required);

	ASSERT_EQ(required.size(), constPrograms.size());
	for (unsigned i = 0; i < required.size(); ++i) {
		EXPECT_EQ(required[i].second, constPrograms.at(required[i].first));
	}
}

TEST(ProgramContainerTest, atInvalid) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	EXPECT_THROW(programs.at(6), InvalidProgramIdException);
}

TEST(ProgramContainerTest, atConstInvalid) {
	const vector<ProgramContainer::value_type> required {
		{10, new Program()},
		{15, new Program()},
		{20, new Program()},
	};

	ProgramContainer programs;
	insertToPrograms(programs, required);

	const ProgramContainer& constPrograms = programs;
	EXPECT_THROW(constPrograms.at(6), InvalidProgramIdException);
}

TEST(ProgramContainerTest, destructed) {
	ProgramContainer programs;
	programs.insert(0, new MockProgram());
}
