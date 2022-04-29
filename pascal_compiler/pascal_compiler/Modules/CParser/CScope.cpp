#include "CScope.h"

CScope::CScope() {
	addIdent("true", "boolean");
	addIdent("false", "boolean");
	addType("integer", SemType::stInt);
	addType("string", SemType::stString);
	addType("real", SemType::stReal);
	addType("boolean", SemType::stBoolean);
}

SemType CScope::getIdentType(string ident) {
	auto identType = idents[ident];
	return types[identType];
}

SemType CScope::getTypeforType(string ident) {
	if (!types.contains(ident))
		return SemType::stError;
	return types[ident];
}

bool CScope::identDefined(string ident) {
	return idents.contains(ident);
}

bool CScope::typeDefined(string ident) {
	return types.contains(ident);
}

void CScope::addIdent(string ident, string identType) {
	idents[ident] = identType;
}

void CScope::addType(string ident, SemType exprType) {
	types[ident] = exprType;
}


