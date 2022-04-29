#include "CVariant.h"


CVariant::CVariant(VariantType variantType) : variantType(variantType) {}

VariantType CVariant::getVariantType() {
	return variantType;
}




CIntVariant::CIntVariant(int value) : CVariant(VariantType::vtInt), value(value) {}

int CIntVariant::getValue() {
	return value;
}

string CIntVariant::toString() {
	return std::to_string(value);
}




CRealVariant::CRealVariant(double value) : CVariant(VariantType::vtReal), value(value) {}

double CRealVariant::getValue() {
	return value;
}

string CRealVariant::toString() {
	return std::to_string(value);
}




CStringVariant::CStringVariant(string value) : CVariant(VariantType::vtString), value(value) {}

string CStringVariant::getValue() {
	return value;
}

string CStringVariant::toString() {
	return value;
}




CBooleanVariant::CBooleanVariant(bool value) : CVariant(VariantType::vtBoolean), value(value) {}

bool CBooleanVariant::getValue() {
	return value;
}

string CBooleanVariant::toString() {
	if (value) return "true";
	return "false";
}





