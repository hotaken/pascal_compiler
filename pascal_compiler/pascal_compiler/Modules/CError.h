#pragma once
#include<exception>
#include<map>
#include<string>
using namespace std;

const int PASCAL_INT_MAX = 214748647;

enum class ErrorCode {
	UnexpectedSymbol = 1000,
	UnknownSymbol,
	InvalidNumber,
	IntegerLimit,
	UnexpectedToken,
	ExpectedToken,
	UnexpectedConstType,
	IdentAlreadyDefined,
	IdentNotDefined,
	TypeMismatch,
};


class CError : public exception {
private:
	string token;
	int line, index;
	ErrorCode errorCode;
public:
	string message;
	explicit CError(ErrorCode errorCode, int line, int index, string token);
	const char* what() const throw() override;
};


const map<ErrorCode, string> errorMessage = {
	{ErrorCode::UnexpectedSymbol, "UnexpectedSymbol"},
	{ErrorCode::UnknownSymbol, "UnknownSymbol"},
	{ErrorCode::InvalidNumber, "InvalidNumber"},
	{ErrorCode::IntegerLimit, "IntegerLimit"},
	{ErrorCode::UnexpectedToken, "UnexpectedToken"},
	{ErrorCode::ExpectedToken, "ExpectedToken"},
	{ErrorCode::UnexpectedConstType, "UnexpectedConstType"},
	{ErrorCode::IdentAlreadyDefined, "IdentAlreadyDefined"},
	{ErrorCode::IdentNotDefined, "IdentNotDefined"},
	{ErrorCode::TypeMismatch, "TypeMismatch"},

};
