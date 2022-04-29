#include "SemType.h"

bool checkSemTypes(SemType first, SemType second) {
	if (first == SemType::stError || second == SemType::stError)
		return true;

	if (first == SemType::stString) {
		return second == SemType::stString;
	}
	if (second == SemType::stString) {
		return first == SemType::stString;
	}

	return true;
}
