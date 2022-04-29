#include<fstream>
#include<iostream>
#include "Modules/IO/io.h"
#include "Modules/CLexer/CLexer.h"
#include "Modules/CParser/CParser.h"

#define FILE_NAME "tests/parser_witherr.pas"


int main(char argc, char** argv) {
	ifstream inputFstream(FILE_NAME);

	unique_ptr<Reader> reader = make_unique<Reader>(inputFstream);
	unique_ptr<Writer> writer = make_unique<Writer>(cout);
	unique_ptr<CLexer> cLexer = make_unique<CLexer>(reader.release());
	unique_ptr<CParser> cParser = make_unique<CParser>(cLexer.release(), writer.release());
	cParser->parse();

	inputFstream.close();
}