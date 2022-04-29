#include "CParser.h"

void CParser::skipTo(bool toIdent, bool toConst, vector<CKeyWords> toKeyWords) {
	while (token != nullptr && !fits(toIdent, toConst, toKeyWords)){
		getNextToken();
	}
}

bool CParser::fits(bool toIdent, bool toConst, vector<CKeyWords> toKeyWords) {
	if (toIdent && token->getType() == TokenType::ttIdent)
		return true;
	if (toConst && token->getType() == TokenType::ttConst)
		return true;
	if (token->getType() == TokenType::ttKeyWord)
		for (auto& keyWord : toKeyWords)
		{
			if (dynamic_pointer_cast<CKeyWordToken>(token)->getKeyWord() == keyWord)
				return true;
		}
	return false;
}