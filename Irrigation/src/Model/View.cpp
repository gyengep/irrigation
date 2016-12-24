/*
 * View.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "View.h"
#include "Document.h"



View::View(Document* document) : document(document) {
	document->addView(this);
}

View::~View() {
}

