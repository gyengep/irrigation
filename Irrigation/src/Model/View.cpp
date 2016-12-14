/*
 * View.cpp
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#include "Commons/common.h"
#include "Model/View.h"
#include "Model/Document.h"

View::View(Document* document) : document(document) {
	document->addView(this);
}

View::~View() {
}

