/*
 * Document_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include "common.h"
#include "Document.h"


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

