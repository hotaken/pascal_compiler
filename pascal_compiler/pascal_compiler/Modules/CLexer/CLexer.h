#pragma once
#include "../IO/io.h"
#include "../CToken.h"
#include "../CVariant.h"
#include "../CError.h"

class CLexer {
public:
	CLexer(Reader* reader);
	unique_ptr<CToken> getNextToken();
private:
	unique_ptr<Reader> reader;
	char ch;
	int line, index;
	void skipComment();
	pair<VariantType, string> getNumber();
	string getFull();
	string getString();
	bool isLetter(char ch);
	bool checkInt(string);
	bool isValidNumber(string& number, int& separators);
	void getNextChar();
};

