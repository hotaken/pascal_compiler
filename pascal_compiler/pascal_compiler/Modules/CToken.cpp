#include "CToken.h"
#include<string>
#include<iostream>
#include<iomanip>

CToken::CToken(TokenType tokenType, int line, int index) : tokenType(tokenType), line(line), index(index) {}

TokenType CToken::getType() {
	return tokenType;
}
pair<int, int> CToken::getPos() {
	pair<int, int> pos = { line, index };
	return pos;
}




CIdentToken::CIdentToken(string name, int line, int index) : CToken(TokenType::ttIdent, line, index), name(name) {}


string CIdentToken::toString() {
	return name + " " + tokenTypeToString.at(getType());
}




CKeyWordToken::CKeyWordToken(CKeyWords name, int line, int index) : CToken(TokenType::ttKeyWord, line, index), name(name) {}

string CKeyWordToken::toString() {
	return keyWordsToString.at(name) + " " + tokenTypeToString.at(getType());
}




CConstToken::CConstToken(CVariant* value, int line, int index) : CToken(TokenType::ttConst, line, index) {
	this->value.reset(value);
}

string CConstToken::toString() {
	return "\"" + value->toString() + "\"" + " " + variantTypeToString.at(value->getVariantType()) + " " + tokenTypeToString.at(getType());
}