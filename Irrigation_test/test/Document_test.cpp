/*
 * Document_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include "gtest/gtest.h"

#include "Document.h"


TEST(Document, ProgramLock) {
	Document doc;

	doc.getProgramMutex().lock();
	EXPECT_FALSE(doc.getProgramMutex().try_lock());
	doc.getProgramMutex().unlock();

	EXPECT_THROW(doc.getPrograms(), not_locked);
	doc.getProgramMutex().unlock();
	EXPECT_THROW(doc.addProgram(), not_locked);
	doc.getProgramMutex().unlock();
	EXPECT_THROW(doc.deleteProgram(0), not_locked);
	doc.getProgramMutex().unlock();
	EXPECT_THROW(doc.moveProgram(0, 0), not_locked);
	doc.getProgramMutex().unlock();
	EXPECT_THROW(doc.getProgram(0), not_locked);
	doc.getProgramMutex().unlock();

}


TEST(Document, addProgram) {
	Document doc;

	AUTO_LOCK_PROGRAMS(&doc);

	EXPECT_EQ(0, doc.getPrograms().size());
	Program& program0 = doc.addProgram();
	EXPECT_EQ(1, doc.getPrograms().size());
	Program& program1 = doc.addProgram();
	EXPECT_EQ(2, doc.getPrograms().size());

	auto programIt = doc.getPrograms().begin();
	EXPECT_EQ(0, programIt->first);
	EXPECT_EQ(&program0, programIt->second);

	++programIt;
	EXPECT_EQ(1, programIt->first);
	EXPECT_EQ(&program1, programIt->second);
}

TEST(Document, deleteProgram) {
	Document doc;

	AUTO_LOCK_PROGRAMS(&doc);

	doc.addProgram();
	doc.addProgram();
	doc.addProgram();

	EXPECT_THROW(doc.deleteProgram(3), invalid_id);
	EXPECT_EQ(3, doc.getPrograms().size());

	EXPECT_NO_THROW(doc.deleteProgram(1));
	EXPECT_EQ(2, doc.getPrograms().size());

	EXPECT_THROW(doc.deleteProgram(1), invalid_id);
	EXPECT_EQ(2, doc.getPrograms().size());

	EXPECT_NO_THROW(doc.deleteProgram(0));
	EXPECT_EQ(1, doc.getPrograms().size());

	EXPECT_NO_THROW(doc.deleteProgram(2));
	EXPECT_EQ(0, doc.getPrograms().size());
}

TEST(Document, moveProgram) {
	Document doc;

	AUTO_LOCK_PROGRAMS(&doc);

	Program* program0 = &doc.addProgram();
	Program* program1 = &doc.addProgram();
	Program* program2 = &doc.addProgram();

	EXPECT_THROW(doc.moveProgram(1, 3), std::out_of_range);
	EXPECT_THROW(doc.moveProgram(3, 1), invalid_id);

	doc.moveProgram(1, 0);

	auto programIt = doc.getPrograms().begin();
	EXPECT_EQ(1, programIt->first);
	EXPECT_EQ(program1, programIt->second);

	++programIt;
	EXPECT_EQ(0, programIt->first);
	EXPECT_EQ(program0, programIt->second);

	++programIt;
	EXPECT_EQ(2, programIt->first);
	EXPECT_EQ(program2, programIt->second);


	doc.moveProgram(1, 2);

	programIt = doc.getPrograms().begin();
	EXPECT_EQ(0, programIt->first);
	EXPECT_EQ(program0, programIt->second);

	++programIt;
	EXPECT_EQ(2, programIt->first);
	EXPECT_EQ(program2, programIt->second);

	++programIt;
	EXPECT_EQ(1, programIt->first);
	EXPECT_EQ(program1, programIt->second);
}

TEST(Document, getProgram) {
	Document doc;

	AUTO_LOCK_PROGRAMS(&doc);

	Program* program0 = &doc.addProgram();
	Program* program1 = &doc.addProgram();
	Program* program2 = &doc.addProgram();

	EXPECT_THROW(doc.getProgram(3), invalid_id);
	EXPECT_EQ(program0, &doc.getProgram(0));
	EXPECT_EQ(program1, &doc.getProgram(1));
	EXPECT_EQ(program2, &doc.getProgram(2));

	const Document& cdoc = doc;
	EXPECT_THROW(cdoc.getProgram(3), invalid_id);
	EXPECT_EQ(program0, &cdoc.getProgram(0));
	EXPECT_EQ(program1, &cdoc.getProgram(1));
	EXPECT_EQ(program2, &cdoc.getProgram(2));
}

TEST(Document, openValve) {
	Document doc;

	for (unsigned i = 0; i < VALVE_COUNT; i++) {
		EXPECT_NO_THROW(doc.openValve(i, true));
	}

	EXPECT_THROW(doc.openValve(VALVE_COUNT, true), std::out_of_range);
}

TEST(Document, openZone) {
	Document doc;

	for (unsigned i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(doc.openZone(i, true));
	}

	EXPECT_THROW(doc.openZone(ZONE_COUNT, true), std::out_of_range);
}

