#include "CParser.h"

CParser::CParser(CLexer* lexer, Writer* writer) {
	this->lexer.reset(lexer);
	this->writer.reset(writer);
	token.reset(this->lexer->getNextToken().release());
}

void CParser::getNextToken() {
	token.reset(this->lexer->getNextToken().release());
}

CKeyWords CParser::getTokenKeyWord() {
	return static_pointer_cast<CKeyWordToken>(token)->getKeyWord();
}

VariantType CParser::getTokenVariantType() {
	return static_pointer_cast<CConstToken>(token)->getVariant()->getVariantType();
}

bool CParser::isKeyWord() {
	return token->getType() == TokenType::ttKeyWord;
}

bool CParser::isIdent() {
	return token->getType() == TokenType::ttIdent;
}

bool CParser::isConst() {
	return token->getType() == TokenType::ttConst;
}

void CParser::passIdent() {
	auto [line, index] = token->getPos();
	if (!isIdent())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	getNextToken();
}

void CParser::passKeyword(CKeyWords keyWord) {
	auto [line, index] = token->getPos();
	if (!isKeyWord())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	if (getTokenKeyWord() != keyWord)
		throw CError(ErrorCode::ExpectedToken, line, index, keyWordsToString.at(keyWord));

	getNextToken();
}

void CParser::passConst(VariantType variantType) {
	auto [line, index] = token->getPos();
	if (!isConst())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	VariantType tokenVariantType = dynamic_pointer_cast<CConstToken>(token)->getVariant()->getVariantType();
	if (tokenVariantType != variantType)
		throw CError(ErrorCode::UnexpectedConstType, line, index, variantTypeToString.at(tokenVariantType));

	getNextToken();
}




void CParser::parse() {
	program(); 
}

void CParser::program() {
	try {
		if (isKeyWord() && getTokenKeyWord() == CKeyWords::programSy) {
			passKeyword(CKeyWords::programSy);
			passIdent();
			passKeyword(CKeyWords::semicolonSy);
		}
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
		skipTo(false, false, { CKeyWords::varSy, CKeyWords::beginSy });
	}
	try {
		block();
		passKeyword(CKeyWords::dotSy);
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
	}
}


void CParser::block() {
	if (isKeyWord() && getTokenKeyWord() == CKeyWords::varSy) {
		varPart();
	}
	statementPart();
}
void CParser::type() {
	passIdent();
}

void CParser::varPart() {
	try {
		passKeyword(CKeyWords::varSy);
		while (isIdent()) {
			varDeclaration();
			passKeyword(CKeyWords::semicolonSy);
		}
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
		skipTo(false, false, { CKeyWords::beginSy });
	}
}


void CParser::varDeclaration() {
	try {
		bool isVarDeclaration = true;
		while (isVarDeclaration) {
			passIdent();
			if (!isKeyWord()) {
				auto [line, index] = token->getPos();
				throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
			}
			if (getTokenKeyWord() == CKeyWords::commaSy) {
				passKeyword(CKeyWords::commaSy);
			}
			else {
				passKeyword(CKeyWords::colonSy);
				isVarDeclaration = false;
			}
		}
		type();
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
		skipTo(false, false, { CKeyWords::semicolonSy });
	}
}

void CParser::statementPart() {
	compoundStatement();
}

void CParser::compoundStatement() {
	passKeyword(CKeyWords::beginSy);

	statement();
	if (!token) 
		return;
	while (isKeyWord() && getTokenKeyWord() == CKeyWords::semicolonSy) {
		passKeyword(CKeyWords::semicolonSy);
		statement();
		if (!token)
			return;
	}

	passKeyword(CKeyWords::endSy);
}

void CParser::statement() {
	try {
		if (isIdent())
			simpleStatement();
		else {
			if (isKeyWord() && structuredStatements.contains(getTokenKeyWord()))
				structuredStatement();
		}
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
		skipTo(true, false, { CKeyWords::semicolonSy, CKeyWords::beginSy, CKeyWords::ifSy, CKeyWords::whileSy, CKeyWords::endSy });
	}
}

void CParser::simpleStatement() {
	passIdent();
	passKeyword(CKeyWords::assignSy);
	expression();
}

void CParser::expression() {
	simpleExpression();
	if (isKeyWord() && relationOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		simpleExpression();
	}
}

void CParser::simpleExpression() {
	term();
	while (isKeyWord() && addingOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		term();
	}
}

void CParser::term() {
	factor();
	while (isKeyWord() && multiplyingOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		factor();
	}
}

void CParser::factor() {
	if (isConst()) {
		passConst(getTokenVariantType());
		return;
	}

	if (isIdent()) {
		passIdent();
		return;
	}

	if (isKeyWord() && unaryOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		factor();
		return;
	}

	passKeyword(CKeyWords::leftBracketSy);
	expression();
	passKeyword(CKeyWords::rightBracketSy);
}

void CParser::structuredStatement() {
	auto [line, index] = token->getPos();
	if (!isKeyWord())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));

	if (getTokenKeyWord() == CKeyWords::ifSy) {
		ifStatement();
		return;
	}

	if (getTokenKeyWord() == CKeyWords::whileSy) {
		whileStatement();
		return;
	}

	if (getTokenKeyWord() == CKeyWords::beginSy) {
		compoundStatement();
		return;
	}
	throw CError(ErrorCode::UnexpectedToken, line, index, keyWordsToString.at(getTokenKeyWord()));
}

void CParser::ifStatement() {
	passKeyword(CKeyWords::ifSy);
	expression();
	passKeyword(CKeyWords::thenSy);
	statement();
	if (isKeyWord() && getTokenKeyWord() == CKeyWords::elseSy) {
		passKeyword(CKeyWords::elseSy);
		statement();
	}
}

void CParser::whileStatement() {
	passKeyword(CKeyWords::whileSy);
	expression();
	passKeyword(CKeyWords::doSy);
	statement();
}