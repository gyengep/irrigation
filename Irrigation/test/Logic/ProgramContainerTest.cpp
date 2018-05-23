#include "ProgramContainerTest.h"
#include <chrono>
#include <thread>
#include "Logic/Exceptions.h"
#include "Logic/Program.h"

using namespace std;
using namespace placeholders;
using testing::_;
using testing::Return;


void ProgramContainerTest::SetUp() {
}

void ProgramContainerTest::TearDown() {

}

bool ProgramContainerTest::ProgramContainerCallback::callback(const IdType id, LockedProgramPtr lockedProgramPtr) {
	LockedProgram* lockedProgram = lockedProgramPtr.get();
	programList.push_back(make_pair(id, lockedProgram->get()));
	return true;
}

const ProgramContainerTest::ProgramList& ProgramContainerTest::ProgramContainerCallback::getProgramList() const {
	return programList;
}

ProgramContainerTest::ProgramList ProgramContainerTest::getAsProgramList(const ProgramContainer* programs) {
	ProgramContainerCallback programContainerCallback;

	auto f = bind(&ProgramContainerCallback::callback, &programContainerCallback, _1, _2);
	programs->iterate(f);

	return programContainerCallback.getProgramList();
}

class MockProgramContainerCallback : public ProgramContainerTest::ProgramContainerCallback {
public:
	MOCK_METHOD2(callback, bool(const IdType, LockedProgramPtr));
};

TEST_F(ProgramContainerTest, iterate) {
	programs.insert(1, new Program());
	programs.insert(2, new Program());
	programs.insert(3, new Program());
	programs.insert(4, new Program());

	MockProgramContainerCallback mockCallback;

	ON_CALL(mockCallback, callback(_, _)).WillByDefault(Return(true));

	EXPECT_CALL(mockCallback, callback(1, _)).Times(1);
	EXPECT_CALL(mockCallback, callback(2, _)).Times(1);
	EXPECT_CALL(mockCallback, callback(3, _)).Times(1);
	EXPECT_CALL(mockCallback, callback(4, _)).Times(1);

	auto f = bind(&ProgramContainerCallback::callback, &mockCallback, _1, _2);

	programs.iterate(f);
}

TEST_F(ProgramContainerTest, iterateAbort) {
	programs.insert(1, new Program());
	programs.insert(2, new Program());
	programs.insert(3, new Program());
	programs.insert(4, new Program());

	MockProgramContainerCallback mockCallback;

	ON_CALL(mockCallback, callback(_, _)).WillByDefault(Return(true));

	EXPECT_CALL(mockCallback, callback(1, _)).Times(1);
	EXPECT_CALL(mockCallback, callback(2, _)).Times(1).WillOnce(Return(false));
	EXPECT_CALL(mockCallback, callback(3, _)).Times(0);
	EXPECT_CALL(mockCallback, callback(4, _)).Times(0);

	auto f = bind(&ProgramContainerCallback::callback, &mockCallback, _1, _2);

	programs.iterate(f);
}

TEST_F(ProgramContainerTest, insert) {
	Program* program1 = new Program();
	Program* program2 = new Program();

	programs.insert(1, program1);
	EXPECT_EQ(ProgramList({{1, program1}}), getAsProgramList(&programs));

	programs.insert(2, program2);
	EXPECT_EQ(ProgramList({{1, program1}, {2, program2}}),getAsProgramList(&programs));
}

TEST_F(ProgramContainerTest, insertExisting) {
	Program* program1 = new Program();
	Program* program2 = new Program();

	EXPECT_NO_THROW(programs.insert(1, program1));
	EXPECT_THROW(programs.insert(1, program2), ProgramIdExist);
}

TEST_F(ProgramContainerTest, erase) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();
	Program* program4 = new Program();

	programs.insert(1, program1);
	programs.insert(2, program2);
	programs.insert(3, program3);
	programs.insert(4, program4);

	EXPECT_EQ(
		ProgramList({{1, program1}, {2, program2}, {3, program3}, {4, program4}}),
		getAsProgramList(&programs));

	// Erase from middle
	programs.erase(2);

	EXPECT_EQ(
		ProgramList({{1, program1}, {3, program3}, {4, program4}}),
		getAsProgramList(&programs));

	// Erase first
	programs.erase(1);

	EXPECT_EQ(
		ProgramList({{3, program3}, {4, program4}}),
		getAsProgramList(&programs));

	// Erase last
	programs.erase(4);

	EXPECT_EQ(
		ProgramList({{3, program3}}),
		getAsProgramList(&programs));

	// Erase last
	programs.erase(3);

	EXPECT_EQ(
		ProgramList(),
		getAsProgramList(&programs));
}

TEST_F(ProgramContainerTest, eraseInvalid) {
	programs.insert(5, new Program());
	EXPECT_THROW(programs.erase(6), InvalidProgramIdException);
}

TEST_F(ProgramContainerTest, get) {
	Program* program1 = new Program();
	Program* program2 = new Program();

	programs.insert(1, program1);
	programs.insert(2, program2);

	EXPECT_EQ(program1, programs.at(1)->get());
	EXPECT_EQ(program2, programs.at(2)->get());
}

TEST_F(ProgramContainerTest, getInvalid) {
	EXPECT_THROW(programs.at(2), InvalidProgramIdException);
}


class MockProgram : public Program {
public:
	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockProgram() { destructorIsCalled(); }
};


TEST_F(ProgramContainerTest, destructed) {
	MockProgram* mockProgram = new MockProgram();
	EXPECT_CALL(*mockProgram, destructorIsCalled()).Times(1);

	{
		ProgramContainer programs;
		programs.insert(0, mockProgram);
	}
}

TEST_F(ProgramContainerTest, eraseDestructed) {
	MockProgram* mockProgram = new MockProgram();
	EXPECT_CALL(*mockProgram, destructorIsCalled()).Times(1);

	programs.insert(0, mockProgram);
	programs.erase(0);
}



void insertPrograms(ProgramContainer* programs, const IdType* ids, size_t count) {
	for(size_t i = 0; i < count; ++i) {
		programs->insert(ids[i], new Program());
	}
}

TEST_F(ProgramContainerTest, concurrentInsert) {
	const size_t threadCount = 5;
	const size_t addCount = 1000;

    thread threads[threadCount];
    vector<array<IdType, addCount>> ids(threadCount);

    IdType id = 0;
    for (size_t i = 0; i < threadCount; ++i) {
    	for (size_t t = 0; t < addCount; ++t) {
    		ids[i][t] = id++;
    	}
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i] = thread(insertPrograms, &programs, ids[i].data(), addCount);
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i].join();
    }

    EXPECT_EQ(threadCount * addCount, programs.size());
}

void erasePrograms(ProgramContainer* programs, const IdType* ids, size_t count) {
	for(size_t i = 0; i < count; ++i) {
		programs->erase(ids[i]);
	}
}

TEST_F(ProgramContainerTest, concurentErase) {
	const size_t threadCount = 5;
	const size_t addCount = 1000;

    thread threads[threadCount];
    vector<array<IdType, addCount>> ids(threadCount);

    IdType id = 0;
    for (size_t i = 0; i < threadCount; ++i) {
    	for (size_t t = 0; t < addCount; ++t) {
    		ids[i][t] = id++;
       		programs.insert(ids[i][t], new Program());
    	}
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i] = thread(erasePrograms, &programs, ids[i].data(), addCount);
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i].join();
    }

    EXPECT_EQ(0, programs.size());
}

void getProgramAndWait(ProgramContainer* programs, IdType id, int ms) {
	LockedProgramPtr lockedProgramPtr = programs->at(id);
    this_thread::sleep_for(chrono::milliseconds(ms));
}

TEST_F(ProgramContainerTest, programLock) {
	const int waitMs = 100;

	programs.insert(1, new Program());
	programs.insert(2, new Program());

	auto start = chrono::high_resolution_clock::now();

	thread thread(getProgramAndWait, &programs, 2, waitMs);
    this_thread::sleep_for(chrono::milliseconds(1));

	LockedProgramPtr program1 = programs.at(1);
	auto finish1 = chrono::high_resolution_clock::now();

	LockedProgramPtr program2 = programs.at(2);
	auto finish2 = chrono::high_resolution_clock::now();

	thread.join();

	auto elapsed1 = chrono::duration_cast<chrono::milliseconds>(finish1 - start);
    EXPECT_LT(elapsed1.count(), waitMs - 10);

	auto elapsed2 = chrono::duration_cast<chrono::milliseconds>(finish2 - start);
    EXPECT_GE(elapsed2.count(), waitMs - 10);
}

