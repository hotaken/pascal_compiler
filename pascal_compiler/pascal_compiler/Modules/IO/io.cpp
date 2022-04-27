#include "io.h"

Reader::Reader(istream& stream) : stream(stream), line(0), index(0) {}

Reader::~Reader() {}

pair<char, pair<int, int>> Reader::getCh() {
	char newch = stream.get();

	pair<char, pair<int, int>> res = { newch, {line, index} };

	if (newch == '\n') {
		line++;
		index = 0;
	}
	else {
		index++;
	}
	return res;
}

Writer::Writer(ostream& stream) : stream(stream) {}

Writer::~Writer() {}

//template<class T> unique_ptr<Writer>& operator << (const unique_ptr<Writer>& writer, const T& t){
//	writer->stream << t;
//	return writer;
//}