#pragma once
#include "CKeyWords.h"
#include "CVariant.h"
#include <memory>
using namespace std;

enum class TokenType {
	ttIdent,
	ttKeyWord,
	ttConst
};


const map<TokenType, string> tokenTypeToString = {
	{TokenType::ttIdent, "ttIdent"},
	{TokenType::ttKeyWord, "ttKeyWord"},
	{TokenType::ttConst, "ttConst"}
};

class CToken {
private:
	TokenType tokenType;
	int line, index;
public:
	CToken(TokenType tType, int line, int index);

	TokenType getType();
	pair<int, int> getPos();

	virtual string toString() PURE;
};


class CIdentToken : public CToken {
private:
	string name;
public:
	CIdentToken(string name, int line, int index);
	string toString() override;
};

class CKeyWordToken : public CToken {
private:
	CKeyWords name;
public:
	CKeyWordToken(CKeyWords name, int line, int index);
	string toString() override;
};

class CConstToken : public CToken {
private:
	unique_ptr<CVariant> value;
public:
	CConstToken(CVariant* value, int line, int index);
	string toString() override;
};