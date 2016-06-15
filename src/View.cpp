/*
 * View.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "common.h"
#include "Document.h"
#include "View.h"

View::View(Document* document) : document(document) {
	document->addView(this);
}

View::~View() {
}

