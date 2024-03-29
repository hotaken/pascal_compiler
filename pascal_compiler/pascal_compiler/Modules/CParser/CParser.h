#pragma once
#include "../CLexer/CLexer.h"
#include <vector>
#include "CScope.h"

class CParser {
public:
	CParser(CLexer* lexer, Writer* writer);
	void parse();

private:
	const set<CKeyWords> structuredStatements = {
		CKeyWords::beginSy,
		CKeyWords::ifSy,
		CKeyWords::whileSy
	};

	const set<CKeyWords> relationOperators = {
		CKeyWords::ltSy,
		CKeyWords::gtSy,
		CKeyWords::leSy,
		CKeyWords::geSy,
		CKeyWords::eqSy,
		CKeyWords::neSy
	};

	const set<CKeyWords> addingOperators = {
		CKeyWords::plusSy,
		CKeyWords::minusSy,
		CKeyWords::orSy
	};

	const set<CKeyWords> multiplyingOperators = {
		CKeyWords::multiplySy,
		CKeyWords::divisionSy,
		CKeyWords::andSy
	};

	const set<CKeyWords> unaryOperators = {
		CKeyWords::plusSy,
		CKeyWords::minusSy,
		CKeyWords::notSy
	};

	shared_ptr<CToken> token;
	unique_ptr<CLexer> lexer;
	unique_ptr<Writer> writer;
	
	unique_ptr<CScope> scope;
	void getNextToken();

	void skipTo(bool, bool, vector<CKeyWords>);
	bool fits(bool, bool, vector<CKeyWords>);

	CKeyWords getTokenKeyWord();
	VariantType getTokenVariantType();

	bool isKeyWord();
	bool isIdent();
	bool isConst();

	shared_ptr<CKeyWordToken> passKeyword(CKeyWords);
	shared_ptr<CConstToken> passConst(VariantType);
	shared_ptr<CIdentToken> passIdent();

	void program();
	void block();
	
	void varPart();
	void varDeclaration();

	void typePart();
	void typeDeclaration();
	shared_ptr<CIdentToken> type();

	void statementPart();
	void compoundStatement();
	void statement();
	void simpleStatement();

	SemType expression();
	SemType simpleExpression();
	SemType term();
	SemType factor();

	void structuredStatement();
	void ifStatement();
	void whileStatement();
};

