#include<fstream>
#include<iostream>
#include "Modules/IO/io.h"
#include "Modules/CLexer/CLexer.h"

#define FILE_NAME "tests/1.pas"


int main(char argc, char** argv) {
	ifstream inputFstream(FILE_NAME);

	unique_ptr<Reader> reader = make_unique<Reader>(inputFstream);
	unique_ptr<Writer> writer = make_unique<Writer>(cout);
	unique_ptr<CLexer> cLexer = make_unique<CLexer>(reader.release());


	while (true) {
		try {
			unique_ptr<CToken> token = cLexer->getNextToken();
			if (token == nullptr) break;
			auto [line, index] = token->getPos();
			writer->stream << line << ":" << index << " " << token->toString() << endl;
		}
		catch (exception& e) {
			writer->stream << "Error: " << e.what() << endl;
		}
		
	}

	inputFstream.close();
}