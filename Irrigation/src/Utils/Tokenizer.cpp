/*
 * Tokenizer.cpp
 *
 *  Created on: 2016.12.14.
 *      Author: 502664609
 */

#include "Utils/Tokenizer.h"


void Tokenizer::tokenize(const std::string& text, StringArray& result, const char* delimeters, bool includeEmpty) {
	result.clear();

	size_t posBegin = 0;
	size_t posEnd;

	while (std::string::npos != (posEnd = text.find_first_of(delimeters, posBegin))) {
		result.push_back(text.substr(posBegin, posEnd - posBegin));
		posBegin = posEnd + 1;
	}

	result.push_back(text.substr(posBegin));
}

/*
void CommandLineView::tokenize(const std::string& text, Tokens& tokens) {
	tokens.clear();

	const char* delimeters = " \t\r\n";
	char* text_copy = new char[text.length() + 1];
	memcpy(text_copy, text.c_str(), text.length() + 1);

	char* pch = strtok(text_copy, delimeters);
	while (pch != NULL) {
		tokens.push_back(pch);
		pch = strtok(NULL, delimeters);
	}

	delete[] text_copy;
}
*/
