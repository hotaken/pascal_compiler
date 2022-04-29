#pragma once
#include <string>
#include <map>
#include "../CVariant.h"

enum class SemType {
	stInt,
	stString,
	stReal,
	stBoolean,
	stError
};

const map<SemType, string> semTypeToString = {
	{SemType::stInt, "integer"},
	{SemType::stString, "string"},
	{SemType::stReal, "real"},
	{SemType::stBoolean, "bolean"},
	{SemType::stError, "error"}
};

const map<VariantType, SemType> variantTypeToSemType{
	{VariantType::vtBoolean, SemType::stBoolean},
	{VariantType::vtInt, SemType::stInt},
	{VariantType::vtReal, SemType::stReal},
	{VariantType::vtString, SemType::stString}
};

bool checkSemTypes(SemType, SemType);
