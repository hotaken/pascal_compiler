#include "CParser.h"

CParser::CParser(CLexer* lexer, Writer* writer) {
	this->lexer.reset(lexer);
	this->writer.reset(writer);
	token.reset(this->lexer->getNextToken().release());

	scope = make_unique<CScope>();
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

shared_ptr<CIdentToken> CParser::passIdent() {
	auto [line, index] = token->getPos();
	if (!isIdent())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	auto identToken = dynamic_pointer_cast<CIdentToken>(token);
	getNextToken();
	return identToken;
}

 shared_ptr<CKeyWordToken> CParser::passKeyword(CKeyWords keyWord) {
	auto [line, index] = token->getPos();
	if (!isKeyWord())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	if (getTokenKeyWord() != keyWord)
		throw CError(ErrorCode::ExpectedToken, line, index, keyWordsToString.at(keyWord));
	auto keywordToken = dynamic_pointer_cast<CKeyWordToken>(token);
	getNextToken();
	return keywordToken;
}

 shared_ptr<CConstToken> CParser::passConst(VariantType variantType) {
	auto [line, index] = token->getPos();
	if (!isConst())
		throw CError(ErrorCode::UnexpectedToken, line, index, tokenTypeToString.at(token->getType()));
	shared_ptr<CConstToken> constToken = std::dynamic_pointer_cast<CConstToken>(token);
	VariantType tokenVariantType = constToken->getVariant()->getVariantType();
	if (tokenVariantType != variantType)
		throw CError(ErrorCode::UnexpectedConstType, line, index, variantTypeToString.at(tokenVariantType));

	getNextToken();
	return constToken;
}




void CParser::parse() {
	program(); 
}

void CParser::program() {
	try {
		if (isKeyWord() && getTokenKeyWord() == CKeyWords::programSy) {
			passKeyword(CKeyWords::programSy);
			auto ident = passIdent();
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
	if (isKeyWord() && getTokenKeyWord() == CKeyWords::typeSy) {
		typePart();
	}
	if (isKeyWord() && getTokenKeyWord() == CKeyWords::varSy) {
		varPart();
	}
	statementPart();
}
shared_ptr<CIdentToken> CParser::type() {
	auto [line, index] = token->getPos();
	if (!token)
		throw CError(ErrorCode::UnexpectedToken, line, index, "eof");
	return passIdent();
}

void CParser::typePart() {
	try {
		passKeyword(CKeyWords::typeSy);
		while (isIdent()) {
			typeDeclaration();
			passKeyword(CKeyWords::semicolonSy);
		}
	}
	catch (CError& e) {
		writer->stream << "Error: " << e.what() << endl;
		skipTo(false, false, { CKeyWords::varSy, CKeyWords::beginSy });
	}
}

void CParser::typeDeclaration() {
	auto [line, index] = token->getPos();
	auto ident = passIdent();
	passKeyword(CKeyWords::eqSy);
	auto typeIdent = type();
	if (scope->identDefined(ident->getName())) {
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::IdentAlreadyDefined) <<
			" " << errorMessage.at(ErrorCode::IdentAlreadyDefined) << endl;
	}
	else {
		if (!scope->typeDefined(typeIdent->getName())) {
			writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::IdentNotDefined) <<
				" " << errorMessage.at(ErrorCode::IdentNotDefined) << endl;
		}
		else {
			auto typeOfType = scope->getTypeforType(typeIdent->getName());
			scope->addType(ident->getName(), typeOfType);
		}
	}
	
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
		auto [line, index] = token->getPos();
		bool isVarDeclaration = true;
		vector<shared_ptr<CIdentToken>> identVec;
		while (isVarDeclaration) {
			identVec.push_back(passIdent());
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
		auto typeIdent = type();
		if (!scope->typeDefined(typeIdent->getName())) {
			writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::IdentNotDefined) <<
				" " << errorMessage.at(ErrorCode::IdentNotDefined) << endl;
		}
		else {
			for (auto ident : identVec) {
				if (scope->identDefined(ident->getName())) {
					writer->stream << "Error: " << line << ":" << index << " " << to_string(static_cast<int>(ErrorCode::IdentAlreadyDefined)) <<
						" " << errorMessage.at(ErrorCode::IdentAlreadyDefined) << endl;
				}
				else {
					scope->addIdent(ident->getName(), typeIdent->getName());
				}
			}
		}
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
	if (!token)
		return;
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
	auto [line, index] = token->getPos();
	auto ident = passIdent();
	SemType identType;
	if (!scope->identDefined(ident->getName())) {
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::IdentNotDefined) <<
			" " << errorMessage.at(ErrorCode::IdentNotDefined) << " " << ident->getName() << endl;
		identType =  SemType::stError;
	}
	else {
		identType = scope->getIdentType(ident->getName());
	}
	passKeyword(CKeyWords::assignSy);


	SemType exprType = expression();
	if (!checkSemTypes(identType, exprType)) {
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
			" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
	}
}

SemType CParser::expression() {
	auto [line, index] = token->getPos();
	SemType leftType = simpleExpression();
	if (isKeyWord() && relationOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		SemType rightType = simpleExpression();
		if (!checkSemTypes(leftType, rightType)) {
			writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
				" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
		}
		if (leftType != SemType::stError)
			leftType = rightType;
	}
	return leftType;
}

SemType CParser::simpleExpression() {
	auto [line, index] = token->getPos();
	SemType leftType = term();
	while (isKeyWord() && addingOperators.contains(getTokenKeyWord())) {
		auto op = getTokenKeyWord();
		passKeyword(getTokenKeyWord());
		SemType rightType = term();
		if (!checkSemTypes(leftType, rightType)) {
			writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
				" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
			leftType = SemType::stError;
		}
		else {
			if (op == CKeyWords::minusSy) {
				if (leftType == SemType::stString) {
					writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
						" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
					leftType = SemType::stError;
				}
				else {
					leftType = rightType;
				}
			}
			if (op == CKeyWords::plusSy) {
				leftType = rightType;
			}
			if (op == CKeyWords::orSy) {
				if (leftType == SemType::stString) {
					writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
						" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
					leftType =SemType::stError;
				}
				else {
					leftType = rightType;
				}
			}
		}
	}
	return leftType;
}

SemType CParser::term() {
	auto [line, index] = token->getPos();
	SemType leftType = factor();
	while (isKeyWord() && multiplyingOperators.contains(getTokenKeyWord())) {
		auto op = getTokenKeyWord();
		passKeyword(getTokenKeyWord());
		SemType rightType = factor();

		if (!checkSemTypes(leftType, rightType)) {
			writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
				" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
			leftType = SemType::stError;
		}
		else {
			if (op == CKeyWords::multiplySy) {
				if (leftType == SemType::stString) {
					writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
						" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
					leftType = SemType::stError;
				}
				else {
					leftType = rightType;
				}
			}
			if (op == CKeyWords::divisionSy) {
				if (leftType == SemType::stString) {
					writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
						" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
					leftType = SemType::stError;
				}
				else {
					leftType = rightType;
				}
			}
			if (op == CKeyWords::andSy) {
				if (leftType == SemType::stString) {
					writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
						" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
					leftType = SemType::stError;
				}
				else {
					leftType = rightType;
				}
			}
		}
	}
	return leftType;
}

SemType CParser::factor() {
	if (isConst()) {
		auto constToken = passConst(getTokenVariantType());
		return variantTypeToSemType.at(constToken->getVariant()->getVariantType());
	}

	if (isIdent()) {
		auto [line, index] = token->getPos();
		auto ident = passIdent();
		if (scope->identDefined(ident->getName())) {
			return scope->getIdentType(ident->getName());
		}
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::IdentNotDefined) <<
			" " << errorMessage.at(ErrorCode::IdentNotDefined) << " " << ident->getName() << endl;
		return SemType::stError;
	}

	if (isKeyWord() && unaryOperators.contains(getTokenKeyWord())) {
		passKeyword(getTokenKeyWord());
		return factor();
	}

	passKeyword(CKeyWords::leftBracketSy);
	SemType expType = expression();
	passKeyword(CKeyWords::rightBracketSy);
	return expType;
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
	auto [line, index] = token->getPos();
	SemType expType = expression();

	if (!checkSemTypes(expType, SemType::stBoolean)) {
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
			" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
	}

	passKeyword(CKeyWords::thenSy);
	statement();
	if (isKeyWord() && getTokenKeyWord() == CKeyWords::elseSy) {
		passKeyword(CKeyWords::elseSy);
		statement();
	}
}

void CParser::whileStatement() {
	passKeyword(CKeyWords::whileSy);
	auto [line, index] = token->getPos();
	SemType expType = expression();

	if (!checkSemTypes(expType, SemType::stBoolean)) {
		writer->stream << "Error: " << line << ":" << index << " " << static_cast<int>(ErrorCode::TypeMismatch) <<
			" " << errorMessage.at(ErrorCode::TypeMismatch) << endl;
	}
	passKeyword(CKeyWords::doSy);
	statement();
}