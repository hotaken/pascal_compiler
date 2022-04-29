#pragma once
#include "SemType.h"
#include <map>
#include <string>

class CScope {
public:
	CScope();
	SemType getIdentType(string ident);
	SemType getTypeforType(string ident);

	bool identDefined(string ident);
	bool typeDefined(string ident);

	void addIdent(string ident, string identType);
	void addType(string ident, SemType exprType);

private:
	map<string, string>idents;
	map<string, SemType> types;
};


