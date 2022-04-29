#pragma once
#include<map>
#include<string>
using namespace std;
#define PURE = 0

enum class VariantType {
	vtInt,
	vtReal,
	vtString,
	vtBoolean
};

class CVariant {
private:
	VariantType variantType;
public:
	CVariant(VariantType variantType);
	VariantType getVariantType();
	virtual string toString() PURE;
};


class CIntVariant : public CVariant {
private:
	int value;
public:
	CIntVariant(int value);
	string toString() override;
	int getValue();
};

class CRealVariant : public CVariant {
private:
	double value;
public:
	CRealVariant(double value);
	string toString() override;
	double getValue();
};

class CStringVariant : public CVariant {
private:
	string value;
public:
	CStringVariant(string value);
	string toString() override;
	string getValue();
};

class CBooleanVariant : public CVariant {
private:
	bool value;
public:
	CBooleanVariant(bool value);
	string toString() override;
	bool getValue();
};

const map<string, VariantType> stringToVariantType = {
	{"integer", VariantType::vtInt},
	{"real", VariantType::vtReal},
	{"string", VariantType::vtString},
	{"boolean", VariantType::vtBoolean}
};

const map<VariantType, string> variantTypeToString = {
	{VariantType::vtInt,"integer"},
	{VariantType::vtReal,"real"},
	{VariantType::vtString,"string"},
	{VariantType::vtBoolean,"boolean"}
};