#include<fstream>
#include<iostream>
#include "Modules/IO/io.h"

#define FILE_NAME "tests/1.pas"


int main(char argc, char** argv) {
	ifstream fstream(FILE_NAME);

	unique_ptr<Reader> reader = make_unique<Reader>(fstream);
	unique_ptr<Writer> writer = make_unique<Writer>(cout);


	while (true) {
		auto [ch, position] = reader->getCh();
		if (ch == EOF)
			break;
		auto [line, index] = position;

		writer->stream << ch << ' ' << line << ' ' << index << endl;
	}


}