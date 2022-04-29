#include "CLexer.h"

CLexer::CLexer(Reader* reader) {
	this->reader.reset(reader);
	tie(ch, line, index) = this->reader->getCh();
}

bool CLexer::isValidNumber(string& number, int& separators) {
	int separatorsCount = 0;
	for (int i = 0; i < number.length(); i++) {
		if (isLetter(number[i]))
			return false;
		if (number[i] == keyWordsToString.at(CKeyWords::dotSy)[0])
			separatorsCount++;
	}
	separators = separatorsCount;
	if (separatorsCount <= 1)
		return true;
	return false;
}

bool CLexer::checkInt(string numberString) {
	int x = 0;
	for (int i = 0; i < numberString.length() - 1; i++){
		if(!((PASCAL_INT_MAX - (numberString[i]-'0'))/10 < x))
			x = x*10 + (numberString[i] - '0');
		else
			return false;
	}
	return true;
}

pair<VariantType, string> CLexer::getNumber() {

	int startLine = line, startIndex = index;

	string numberString;
	while (isdigit(ch) || isLetter(ch) || ch == '.') {
		numberString += ch;
		getNextChar();
	}
	int separators = 0;
	if (!isValidNumber(numberString, separators))
		throw CError(ErrorCode::InvalidNumber, startLine, startIndex, numberString);

	if (separators > 0) {
		return { VariantType::vtReal, numberString };
	}
	else {
		if (!checkInt(numberString))
			throw CError(ErrorCode::IntegerLimit, startLine, startIndex, numberString);
		return { VariantType::vtInt, numberString };
	}
}

void CLexer::skipComment() {
	int startLine = line, startIndex = index;

	while (ch != '}' && ch != EOF)
		getNextChar();

	if (ch == EOF)
		throw CError(ErrorCode::UnexpectedSymbol, startLine, startIndex, "{");
	else
		getNextChar();
}

string CLexer::getString() {
	int startLine = line, startIndex = index;

	string resString;
	getNextChar();

	while (ch != '\n') {
		resString += ch;
		getNextChar();
		if (ch == '\'') {
			getNextChar();
			break;
		}
		if (ch == '\n')
			throw CError(ErrorCode::UnexpectedSymbol, startLine, startIndex, "\'");
	}

	return resString;
}


void CLexer::getNextChar() {
	tie(ch, line, index) = reader->getCh();
}
bool CLexer::isLetter(char ch) {
	return  ch == '_' || ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z';
}

string CLexer::getFull() {
	string full;
	while (isdigit(ch) || isLetter(ch)) {
		full += ch;
		getNextChar();
	}

	return full;
}

unique_ptr<CToken> CLexer::getNextToken() {

	if (ch == EOF)
		return nullptr;
	while (isspace(ch) || ch == '{') {
		if (ch == '{')
			skipComment();
		else
			getNextChar();
	}

	int startLine = line, startIndex = index;

	if (ch == '}')
		throw CError(ErrorCode::UnexpectedSymbol, startLine, startIndex, "}");

	if (isdigit(ch)) {
		auto [type, number] = getNumber();

		if (type == VariantType::vtInt) {
			int value = static_cast<int>(stoll(number));
			unique_ptr<CVariant> intVariant = make_unique<CIntVariant>(value);
			return make_unique<CConstToken>(intVariant.release(), startLine, startIndex);
		}
		else {
			double value = stod(number);
			unique_ptr<CVariant> realVariant = make_unique<CRealVariant>(value);
			return make_unique<CConstToken>(realVariant.release(), startLine, startIndex);
		}
	}

	if (isLetter(ch)) {
		string name = getFull();

		if (isKeyWord(name)) {
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
		else
			if (name == "true" || name == "false") {
				bool value = (name == "true" ? true : false);
				unique_ptr<CVariant> boolVariant = make_unique<CBooleanVariant>(value);
				return make_unique<CConstToken>(boolVariant.release(), startLine, startIndex);
			}
			else {
				return make_unique<CIdentToken>(name, startLine, startIndex);
			}
	}

	if (ch == '\'') {
		string value = getString();
		unique_ptr<CVariant> stringVariant = make_unique<CStringVariant>(value);
		return make_unique<CConstToken>(stringVariant.release(), startLine, startIndex);
	}

	if (ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
		ch == '(' || ch == ')' || ch == '.' || ch == ',' || ch == ';' )
	{
		string name;
		name += ch;
		getNextChar();
		return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
	}

	string name;
	name += ch;

	char prevCh = ch;
	getNextChar();

	switch (prevCh) {
	case '<':
		if (ch == '=' || ch == '>') {
			name += ch;
			getNextChar();
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
		else {
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
	case '>':
		if (ch == '=') {
			name += ch;
			getNextChar();
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
		else {
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
	case ':':
		if (ch == '=') {
			name += ch;
			getNextChar();
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
		else {
			return make_unique<CKeyWordToken>(stringToKeyWords.at(name), startLine, startIndex);
		}
	default:
		throw CError(ErrorCode::UnknownSymbol, startLine, startIndex, name);
	}

	return nullptr;
}