/*
 * WebServer_test.cpp
 *
 *  Created on: 2016.12.13.
 *      Author: 502664609
 */

#include "Commons/common.h"
#include "Utils/Tokenizer.h"


TEST(Tokenizer, tokenize) {

	StringArray stringArray;

	Tokenizer::tokenize("newwindow=1&safe=active&espv=2&q=c%2B%2B+runtime_error", stringArray, "&", true);
	EXPECT_EQ(4, stringArray.size());
	EXPECT_EQ("newwindow=1", stringArray[0]);
	EXPECT_EQ("safe=active", stringArray[1]);
	EXPECT_EQ("espv=2", stringArray[2]);
	EXPECT_EQ("q=c%2B%2B+runtime_error", stringArray[3]);

	Tokenizer::tokenize("", stringArray, "&", true);
	EXPECT_EQ(1, stringArray.size());

	Tokenizer::tokenize("&", stringArray, "&", true);
	EXPECT_EQ(2, stringArray.size());
	EXPECT_EQ("", stringArray[0]);
	EXPECT_EQ("", stringArray[1]);

	Tokenizer::tokenize("abc=5&", stringArray, "&", true);
	EXPECT_EQ(2, stringArray.size());
	EXPECT_EQ("abc=5", stringArray[0]);
	EXPECT_EQ("", stringArray[1]);

	Tokenizer::tokenize("&abc=5", stringArray, "&", true);
	EXPECT_EQ(2, stringArray.size());
	EXPECT_EQ("", stringArray[0]);
	EXPECT_EQ("abc=5", stringArray[1]);

	Tokenizer::tokenize("&&abc=5", stringArray, "&", true);
	EXPECT_EQ(3, stringArray.size());
	EXPECT_EQ("", stringArray[0]);
	EXPECT_EQ("", stringArray[1]);
	EXPECT_EQ("abc=5", stringArray[2]);
}

