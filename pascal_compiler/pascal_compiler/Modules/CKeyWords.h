#pragma once
#include<map>
#include<string>
#include<set>
using namespace std;

enum class CKeyWords {
	programSy, // program 
	beginSy, // begin 
	endSy, // end 
	varSy, // var 
	plusSy, // +
	minusSy, // - 
	multiplySy, // *
	divisionSy,	// /
	leftBracketSy, // (
	rightBracketSy, // )
	assignSy, // :=
	dotSy, // .
	commaSy, // ,
	colonSy, // :
	semicolonSy, // ;
	eqSy, // =
	ltSy, // <
	gtSy, // >
	leSy, // <=
	geSy, // >=
	neSy, // <>
	ifSy, // if 
	thenSy, // then 
	elseSy, // else 
	whileSy, // while 
	doSy, // do 
	andSy, // and 
	orSy, // or 
	xorSy, // xor 
	notSy, // not 
	startCommentSy, // {
	endCommentSy, // }

};

const map<string, CKeyWords> stringToKeyWords = {
	{"program", CKeyWords::programSy},
	{"begin", CKeyWords::beginSy},
	{"end", CKeyWords::endSy},
	{"var", CKeyWords::varSy},
	{"+", CKeyWords::plusSy},
	{"-", CKeyWords::minusSy},
	{"*", CKeyWords::multiplySy},
	{"/", CKeyWords::divisionSy},
	{"(", CKeyWords::leftBracketSy},
	{")", CKeyWords::rightBracketSy},
	{":=", CKeyWords::assignSy},
	{".", CKeyWords::dotSy},
	{",", CKeyWords::commaSy},
	{":", CKeyWords::colonSy},
	{";", CKeyWords::semicolonSy},
	{"=", CKeyWords::eqSy},
	{"<", CKeyWords::ltSy},
	{">", CKeyWords::gtSy},
	{"<=", CKeyWords::leSy},
	{">=", CKeyWords::geSy},
	{"<>", CKeyWords::neSy},
	{"if", CKeyWords::ifSy},
	{"then", CKeyWords::thenSy},
	{"else", CKeyWords::elseSy},
	{"while", CKeyWords::whileSy},
	{"do", CKeyWords::doSy},
	{"and", CKeyWords::andSy},
	{"or", CKeyWords::orSy},
	{"xor", CKeyWords::xorSy},
	{"not", CKeyWords::notSy},
	
};

const map<CKeyWords, string> keyWordsToString = {
	{CKeyWords::programSy, "program"},
	{CKeyWords::beginSy, "begin"},
	{CKeyWords::endSy, "end"},
	{CKeyWords::varSy, "var"},
	{CKeyWords::plusSy, "+"},
	{CKeyWords::minusSy, "-"},
	{CKeyWords::multiplySy, "*"},
	{CKeyWords::divisionSy, "/"},
	{CKeyWords::leftBracketSy, "("},
	{CKeyWords::rightBracketSy, ")"},
	{CKeyWords::assignSy, ":="},
	{CKeyWords::dotSy, "."},
	{CKeyWords::commaSy, ","},
	{CKeyWords::colonSy, ":"},
	{CKeyWords::semicolonSy, ";"},
	{CKeyWords::eqSy, "="},
	{CKeyWords::ltSy, "<"},
	{CKeyWords::gtSy, ">"},
	{CKeyWords::leSy, "<="},
	{CKeyWords::geSy, ">=" },
	{CKeyWords::neSy, "<>"},
	{CKeyWords::ifSy, "if"},
	{CKeyWords::thenSy, "then"},
	{CKeyWords::elseSy, "else"},
	{CKeyWords::whileSy, "while"},
	{CKeyWords::doSy, "do"},
	{CKeyWords::andSy, "and"},
	{CKeyWords::orSy, "or"},
	{CKeyWords::xorSy, "xor"},
	{CKeyWords::notSy, "not"},
};

bool isKeyWord(string);