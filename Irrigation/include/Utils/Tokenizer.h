/*
 * Tokenizer.h
 *
 *  Created on: 2016.12.14.
 *      Author: 502664609
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_


#include <string>
#include <vector>


typedef std::vector<std::string> StringArray;

class Tokenizer {
public:

	static void tokenize(const std::string& text, StringArray& result, const char* delimeter, bool includeEmpty);
};

#endif /* TOKENIZER_H_ */
