/*
 * View.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef VIEW_H_
#define VIEW_H_

class Document;


class View {
	Document* document;
	
public:
	View(Document* document);
	virtual ~View();

	virtual void update() {}
	
	Document* getDocument() { return document; }
	const Document* getDocument() const { return document; }
};

#endif /* VIEW_H_ */
