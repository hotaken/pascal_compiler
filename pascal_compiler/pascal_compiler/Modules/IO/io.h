#pragma once

#include<string>
#include<utility>
#include<istream>
#include<ostream>
using namespace std;

class Reader {
public:
	Reader(istream& stream);
	~Reader();
	pair<char, pair<int, int>> getCh();
private:
	istream& stream;
	int line, index;
};

class Writer {
public:
	Writer(ostream& stream);
	~Writer();
	/*template<class T> friend unique_ptr<Writer>& operator << (const unique_ptr<Writer>&,const T& t);*/
	ostream& stream;
};
