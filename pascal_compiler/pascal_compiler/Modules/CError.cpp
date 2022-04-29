#include "CError.h"

CError::CError(ErrorCode errorCode, int line, int index, string token)
	:errorCode(errorCode), line(line), index(index), token(token) {
	message = to_string(line) + ":" + to_string(index) + " " +
		to_string(static_cast<int>(errorCode)) + " " +
		errorMessage.at(errorCode) + "\'" + token + "\'";
}

const char* CError::what() const throw() {
	return message.data();
}